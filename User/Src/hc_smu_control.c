/*
 * hc_smu_control.c
 *
 * 四通道HC-SMU校准板控制模块实现
 * 功能：电流源/电压源测量与输出控制
 *
 * Created on: Jan 03, 2026
 * Author: Claude Code
 */

#include "hc_smu_control.h"
#include "gpio.h"
#include <string.h>

/*=============================================================================
 * 私有宏定义
 *============================================================================*/
#define RESISTOR_PIN_COUNT    7    // 档位控制引脚数量

/*=============================================================================
 * 私有类型定义 - 档位引脚配置表
 *============================================================================*/
typedef struct {
    GPIO_TypeDef* port;    // GPIO端口
    uint16_t pin;          // GPIO引脚
} GPIO_Pin_t;

/*=============================================================================
 * 私有常量 - 档位引脚映射表
 * 顺序: [RES, RES01, RES02, RES03, RES04, RES05, RES06]
 *============================================================================*/
static const GPIO_Pin_t RESISTOR_PINS[RESISTOR_PIN_COUNT] = {
    {RELAY_RES_GPIO_Port,   RELAY_RES_Pin},    // RES    (PA1)
    {RELAY_RES01_GPIO_Port, RELAY_RES01_Pin},  // RES01  (PC1)
    {RELAY_RES02_GPIO_Port, RELAY_RES02_Pin},  // RES02  (PC2)
    {RELAY_RES03_GPIO_Port, RELAY_RES03_Pin},  // RES03  (PC3)
    {RELAY_RES04_GPIO_Port, RELAY_RES04_Pin},  // RES04  (PC4)
    {RELAY_RES05_GPIO_Port, RELAY_RES05_Pin},  // RES05  (PC5)
    {RELAY_RES06_GPIO_Port, RELAY_RES06_Pin}   // RES06  (PB0)
};

/*=============================================================================
 * 私有常量 - 档位配置表
 * 每个档位对应的7位二进制配置 (bit6~bit0 对应 RES~RES06)
 *============================================================================*/
static const uint8_t GEAR_CONFIG_TABLE[RESISTOR_GEAR_COUNT] = {
    0b0000001,    // GEAR_1A    (档位1: 1A,    5Ω)
    0b0000010,    // GEAR_200MA (档位2: 200mA, 25Ω)
    0b0000100,    // GEAR_20MA  (档位3: 20mA,  250Ω)
    0b0001000,    // GEAR_2MA   (档位4: 2mA,   2.5KΩ)
    0b0010000,    // GEAR_200UA (档位5: 200uA, 25KΩ)
    0b0100000     // GEAR_20UA  (档位6: 20uA,  250KΩ)
};

/*=============================================================================
 * 私有常量 - 通道引脚映射表
 *============================================================================*/
static const GPIO_Pin_t CHANNEL_PINS[CHANNEL_COUNT] = {
    {RELAY_CH01_GPIO_Port, RELAY_CH01_Pin},  // CHANNEL_1 (PA4)
    {RELAY_CH02_GPIO_Port, RELAY_CH02_Pin},  // CHANNEL_2 (PA5)
    {RELAY_CH03_GPIO_Port, RELAY_CH03_Pin},  // CHANNEL_3 (PA6)
    {RELAY_CH04_GPIO_Port, RELAY_CH04_Pin}   // CHANNEL_4 (PA7)
};

/*=============================================================================
 * 全局变量 - 系统状态
 *============================================================================*/
HC_SMU_State_t g_hc_smu_state = {
    .current_channel = CHANNEL_INVALID,
    .current_gear = GEAR_INVALID,
    .current_mode = MODE_INVALID,
    .mos_state = MOS_OFF,
    .is_initialized = 0
};

/*=============================================================================
 * 私有函数声明
 *============================================================================*/
static void HC_SMU_SetGPIO(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state);
static GPIO_PinState HC_SMU_ReadGPIO(GPIO_TypeDef* port, uint16_t pin);
static void HC_SMU_ApplyGearConfig(uint8_t gear_bitmap);

/*=============================================================================
 * 函数实现 - 初始化模块
 *============================================================================*/

/**
 * @brief  HC-SMU控制模块初始化
 */
HC_SMU_Status_t HC_SMU_Init(void)
{
    // 关闭所有通道
    HC_SMU_CloseAllChannels();

    // 关闭所有负载电阻
    HC_SMU_DisableAllResistors();

    // 关闭MOS管 (拉低HF至COM)
    HC_SMU_SetMOSState(MOS_OFF);

    // 设置默认模式为电压源模式
    g_hc_smu_state.current_mode = MODE_VOLTAGE_SOURCE;

    // 关闭OS继电器 (RELAY_OS控制电流源/电压源输出短接到地)
    HC_SMU_SetGPIO(RELAY_OS_GPIO_Port, RELAY_OS_Pin, GPIO_PIN_RESET);

    // 标记为已初始化
    g_hc_smu_state.is_initialized = 1;

    return HC_SMU_OK;
}

/**
 * @brief  系统复位后的状态恢复
 */
HC_SMU_Status_t HC_SMU_Reset(void)
{
    // 重新初始化所有状态
    g_hc_smu_state.is_initialized = 0;
    return HC_SMU_Init();
}

/*=============================================================================
 * 函数实现 - MOS管控制模块
 *============================================================================*/

/**
 * @brief  设置MOS管状态
 */
HC_SMU_Status_t HC_SMU_SetMOSState(MOS_State_t state)
{
    if (state != MOS_ON && state != MOS_OFF) {
        return HC_SMU_ERR_INVALID_PARAM;
    }

    // MOS_ON: 高电平, HF正常工作
    // MOS_OFF: 低电平, 拉低HF至COM, HF停止工作
    GPIO_PinState pin_state = (state == MOS_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HC_SMU_SetGPIO(MOS_GPIO_Port, MOS_Pin, pin_state);

    g_hc_smu_state.mos_state = state;

    return HC_SMU_OK;
}

/**
 * @brief  获取当前MOS管状态
 */
MOS_State_t HC_SMU_GetMOSState(void)
{
    return g_hc_smu_state.mos_state;
}

/*=============================================================================
 * 函数实现 - 通道切换模块
 *============================================================================*/

/**
 * @brief  切换到指定通道
 */
HC_SMU_Status_t HC_SMU_SelectChannel(Channel_Select_t channel)
{
    // 参数检查
    if (channel >= CHANNEL_COUNT) {
        return HC_SMU_ERR_INVALID_CHANNEL;
    }

    // 先关闭所有通道，避免干扰
    HC_SMU_CloseAllChannels();

    // 短暂延时，确保继电器完全断开
    for (volatile uint32_t i = 0; i < 10000; i++);

    // 打开指定通道
    HC_SMU_SetGPIO(CHANNEL_PINS[channel].port,
                   CHANNEL_PINS[channel].pin,
                   GPIO_PIN_SET);

    g_hc_smu_state.current_channel = channel;

    return HC_SMU_OK;
}

/**
 * @brief  关闭所有通道
 */
HC_SMU_Status_t HC_SMU_CloseAllChannels(void)
{
    for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
        HC_SMU_SetGPIO(CHANNEL_PINS[i].port,
                       CHANNEL_PINS[i].pin,
                       GPIO_PIN_RESET);
    }

    g_hc_smu_state.current_channel = CHANNEL_INVALID;

    return HC_SMU_OK;
}

/**
 * @brief  获取当前选中的通道
 */
Channel_Select_t HC_SMU_GetCurrentChannel(void)
{
    return g_hc_smu_state.current_channel;
}

/*=============================================================================
 * 函数实现 - 模式配置模块
 *============================================================================*/

/**
 * @brief  设置测量模式
 */
HC_SMU_Status_t HC_SMU_SetMeasureMode(Measure_Mode_t mode)
{
    if (mode != MODE_CURRENT_SOURCE && mode != MODE_VOLTAGE_SOURCE) {
        return HC_SMU_ERR_INVALID_MODE;
    }

    // 模式切换前先关闭所有负载
    if (g_hc_smu_state.current_mode != mode) {
        HC_SMU_DisableAllResistors();

        // 短暂延时
        for (volatile uint32_t i = 0; i < 10000; i++);
    }

    g_hc_smu_state.current_mode = mode;

    // 测电流源模式: HF_I、HS_I、LS直接接万用表，不经过负载
    // 测电压源模式: 需经过选定负载接入万用表
    // 注: 负载选择通过HC_SMU_SetResistorGear()函数控制

    return HC_SMU_OK;
}

/**
 * @brief  获取当前测量模式
 */
Measure_Mode_t HC_SMU_GetMeasureMode(void)
{
    return g_hc_smu_state.current_mode;
}

/*=============================================================================
 * 函数实现 - 档位选择模块
 *============================================================================*/

/**
 * @brief  设置负载电阻档位
 */
HC_SMU_Status_t HC_SMU_SetResistorGear(Resistor_Gear_t gear)
{
    // 参数检查
    if (gear >= RESISTOR_GEAR_COUNT) {
        return HC_SMU_ERR_INVALID_GEAR;
    }

    // 先关闭所有负载，避免切换冲突
    HC_SMU_DisableAllResistors();

    // 短暂延时，确保继电器完全断开
    for (volatile uint32_t i = 0; i < 10000; i++);

    // 应用新的档位配置
    HC_SMU_ApplyGearConfig(GEAR_CONFIG_TABLE[gear]);

    g_hc_smu_state.current_gear = gear;

    return HC_SMU_OK;
}

/**
 * @brief  获取当前负载电阻档位
 */
Resistor_Gear_t HC_SMU_GetResistorGear(void)
{
    return g_hc_smu_state.current_gear;
}

/**
 * @brief  关闭所有负载电阻
 */
HC_SMU_Status_t HC_SMU_DisableAllResistors(void)
{
    for (uint8_t i = 0; i < RESISTOR_PIN_COUNT; i++) {
        HC_SMU_SetGPIO(RESISTOR_PINS[i].port,
                       RESISTOR_PINS[i].pin,
                       GPIO_PIN_RESET);
    }

    g_hc_smu_state.current_gear = GEAR_INVALID;

    return HC_SMU_OK;
}

/*=============================================================================
 * 函数实现 - 综合控制函数
 *============================================================================*/

/**
 * @brief  配置完整的测量参数
 * @note   配置顺序: 关闭MOS -> 关闭通道 -> 设置模式 -> 设置档位 -> 选择通道 -> 打开MOS
 */
HC_SMU_Status_t HC_SMU_ConfigMeasure(Channel_Select_t channel,
                                      Measure_Mode_t mode,
                                      Resistor_Gear_t gear,
                                      MOS_State_t mos_state)
{
    HC_SMU_Status_t status;

    // 1. 先关闭MOS管，确保安全
    status = HC_SMU_SetMOSState(MOS_OFF);
    if (status != HC_SMU_OK) return status;

    // 2. 关闭所有通道
    status = HC_SMU_CloseAllChannels();
    if (status != HC_SMU_OK) return status;

    // 3. 设置测量模式
    status = HC_SMU_SetMeasureMode(mode);
    if (status != HC_SMU_OK) return status;

    // 4. 如果是电压源模式，设置负载档位
    if (mode == MODE_VOLTAGE_SOURCE) {
        status = HC_SMU_SetResistorGear(gear);
        if (status != HC_SMU_OK) return status;
    } else {
        // 电流源模式不需要负载，关闭所有负载
        HC_SMU_DisableAllResistors();
    }

    // 5. 选择通道
    status = HC_SMU_SelectChannel(channel);
    if (status != HC_SMU_OK) return status;

    // 6. 根据需要打开MOS管
    status = HC_SMU_SetMOSState(mos_state);
    if (status != HC_SMU_OK) return status;

    return HC_SMU_OK;
}

/**
 * @brief  获取系统当前完整状态
 */
HC_SMU_State_t* HC_SMU_GetSystemState(void)
{
    return &g_hc_smu_state;
}

/*=============================================================================
 * 函数实现 - 辅助调试函数
 *============================================================================*/

/**
 * @brief  获取档位描述字符串
 */
const char* HC_SMU_GetGearDescription(Resistor_Gear_t gear)
{
    static const char* gear_desc[] = {
        "Gear1: 1A, 5Ohm, 5W",
        "Gear2: 200mA, 25Ohm, 1W",
        "Gear3: 20mA, 250Ohm, 100mW",
        "Gear4: 2mA, 2.5KOhm, 10mW",
        "Gear5: 200uA, 25KOhm, 1mW",
        "Gear6: 20uA, 250KOhm, 0.1mW"
    };

    if (gear < RESISTOR_GEAR_COUNT) {
        return gear_desc[gear];
    }
    return "Invalid Gear";
}

/**
 * @brief  获取模式描述字符串
 */
const char* HC_SMU_GetModeDescription(Measure_Mode_t mode)
{
    static const char* mode_desc[] = {
        "Current Source Mode",
        "Voltage Source Mode"
    };

    if (mode == MODE_CURRENT_SOURCE || mode == MODE_VOLTAGE_SOURCE) {
        return mode_desc[mode];
    }
    return "Invalid Mode";
}

/*=============================================================================
 * 私有函数实现 - GPIO操作封装
 *============================================================================*/

/**
 * @brief  设置GPIO引脚状态
 */
static void HC_SMU_SetGPIO(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(port, pin, state);
}

/**
 * @brief  读取GPIO引脚状态
 */
static GPIO_PinState HC_SMU_ReadGPIO(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

/**
 * @brief  应用档位配置位图到GPIO引脚
 * @param  gear_bitmap: 7位二进制配置 (bit6~bit0 对应 RES~RES06)
 */
static void HC_SMU_ApplyGearConfig(uint8_t gear_bitmap)
{
    for (uint8_t i = 0; i < RESISTOR_PIN_COUNT; i++) {
        // 从最低位开始，bit0对应RES06, bit6对应RES
        uint8_t bit_index = RESISTOR_PIN_COUNT - 1 - i;
        GPIO_PinState state = (gear_bitmap & (1 << bit_index)) ? GPIO_PIN_SET : GPIO_PIN_RESET;

        HC_SMU_SetGPIO(RESISTOR_PINS[i].port,
                       RESISTOR_PINS[i].pin,
                       state);
    }
}
