/*
 * hc_smu_control.h
 *
 * 四通道HC-SMU校准板控制模块
 * 功能：电流源/电压源测量与输出控制
 *
 * Created on: Jan 03, 2026
 * Author: Claude Code
 */

#ifndef INC_HC_SMU_CONTROL_H_
#define INC_HC_SMU_CONTROL_H_

#include "main.h"

/*=============================================================================
 * 宏定义 - 档位配置
 *============================================================================*/
#define RESISTOR_GEAR_COUNT    6    // 档位总数

// 档位枚举定义
typedef enum {
    GEAR_1A    = 0,    // 档位1: 0~5V, 1A,    5Ω,    5W
    GEAR_200MA = 1,    // 档位2: 0~5V, 200mA, 25Ω,   1W
    GEAR_20MA  = 2,    // 档位3: 0~5V, 20mA,  250Ω,  100mW
    GEAR_2MA   = 3,    // 档位4: 0~5V, 2mA,   2.5KΩ, 10mW
    GEAR_200UA = 4,    // 档位5: 0~5V, 200uA, 25KΩ,  1mW
    GEAR_20UA  = 5,    // 档位6: 0~5V, 20uA,  250KΩ, 0.1mW
    GEAR_INVALID = 0xFF
} Resistor_Gear_t;

/*=============================================================================
 * 宏定义 - 通道配置
 *============================================================================*/
#define CHANNEL_COUNT    4    // 通道总数

// 通道枚举定义
typedef enum {
    CHANNEL_1 = 0,    // 通道1
    CHANNEL_2 = 1,    // 通道2
    CHANNEL_3 = 2,    // 通道3
    CHANNEL_4 = 3,    // 通道4
    CHANNEL_INVALID = 0xFF
} Channel_Select_t;

/*=============================================================================
 * 宏定义 - 测量模式
 *============================================================================*/
// 测量模式枚举定义
typedef enum {
    MODE_CURRENT_SOURCE = 0,    // 测电流源模式: HF_I、HS_I、LS直接接万用表
    MODE_VOLTAGE_SOURCE = 1,    // 测电压源模式: 需经过负载接万用表
    MODE_INVALID = 0xFF
} Measure_Mode_t;

/*=============================================================================
 * 宏定义 - MOS管控制状态
 *============================================================================*/
// MOS管状态枚举定义
typedef enum {
    MOS_OFF = 0,    // MOS关闭: 拉低HF至COM，HF停止工作
    MOS_ON  = 1     // MOS开启: HF正常工作
} MOS_State_t;

/*=============================================================================
 * 宏定义 - 控制状态返回值
 *============================================================================*/
typedef enum {
    HC_SMU_OK = 0,              // 操作成功
    HC_SMU_ERR_INVALID_GEAR,    // 无效档位
    HC_SMU_ERR_INVALID_CHANNEL, // 无效通道
    HC_SMU_ERR_INVALID_MODE,    // 无效模式
    HC_SMU_ERR_INVALID_PARAM    // 无效参数
} HC_SMU_Status_t;

/*=============================================================================
 * 数据结构 - 系统状态
 *============================================================================*/
typedef struct {
    Channel_Select_t   current_channel;    // 当前选中的通道
    Resistor_Gear_t    current_gear;       // 当前选中的档位
    Measure_Mode_t     current_mode;       // 当前测量模式
    MOS_State_t        mos_state;          // MOS管状态
    uint8_t            is_initialized;     // 初始化标志
} HC_SMU_State_t;

/*=============================================================================
 * 全局变量声明
 *============================================================================*/
extern HC_SMU_State_t g_hc_smu_state;

/*=============================================================================
 * 函数声明 - 初始化模块
 *============================================================================*/

/**
 * @brief  HC-SMU控制模块初始化
 * @note   必须在使用其他功能前调用此函数
 * @retval HC_SMU_OK: 初始化成功
 */
HC_SMU_Status_t HC_SMU_Init(void);

/**
 * @brief  系统复位后的状态恢复
 * @note   用于上电复位或按键复位后恢复默认状态
 * @retval HC_SMU_OK: 复位成功
 */
HC_SMU_Status_t HC_SMU_Reset(void);

/*=============================================================================
 * 函数声明 - MOS管控制模块
 *============================================================================*/

/**
 * @brief  设置MOS管状态
 * @param  state: MOS_ON(高电平,HF正常工作) / MOS_OFF(低电平,HF停止工作)
 * @retval HC_SMU_OK: 操作成功
 *         HC_SMU_ERR_INVALID_PARAM: 无效参数
 */
HC_SMU_Status_t HC_SMU_SetMOSState(MOS_State_t state);

/**
 * @brief  获取当前MOS管状态
 * @retval MOS_ON / MOS_OFF
 */
MOS_State_t HC_SMU_GetMOSState(void);

/*=============================================================================
 * 函数声明 - 通道切换模块
 *============================================================================*/

/**
 * @brief  切换到指定通道
 * @param  channel: CHANNEL_1 ~ CHANNEL_4
 * @note   切换通道时会自动关闭其他通道，避免干扰
 * @retval HC_SMU_OK: 切换成功
 *         HC_SMU_ERR_INVALID_CHANNEL: 无效通道号
 */
HC_SMU_Status_t HC_SMU_SelectChannel(Channel_Select_t channel);

/**
 * @brief  关闭所有通道
 * @note   用于测量完成后或切换通道前关闭所有输出
 * @retval HC_SMU_OK: 操作成功
 */
HC_SMU_Status_t HC_SMU_CloseAllChannels(void);

/**
 * @brief  获取当前选中的通道
 * @retval 当前通道号 (CHANNEL_1 ~ CHANNEL_4)
 */
Channel_Select_t HC_SMU_GetCurrentChannel(void);

/*=============================================================================
 * 函数声明 - 模式配置模块
 *============================================================================*/

/**
 * @brief  设置测量模式
 * @param  mode: MODE_CURRENT_SOURCE(测电流源) / MODE_VOLTAGE_SOURCE(测电压源)
 * @note   测电流源模式: HF_I、HS_I、LS直接接万用表，不经过负载
 *         测电压源模式: 需经过选定负载接入万用表
 * @retval HC_SMU_OK: 切换成功
 *         HC_SMU_ERR_INVALID_MODE: 无效模式
 */
HC_SMU_Status_t HC_SMU_SetMeasureMode(Measure_Mode_t mode);

/**
 * @brief  获取当前测量模式
 * @retval MODE_CURRENT_SOURCE / MODE_VOLTAGE_SOURCE
 */
Measure_Mode_t HC_SMU_GetMeasureMode(void);

/*=============================================================================
 * 函数声明 - 档位选择模块
 *============================================================================*/

/**
 * @brief  设置负载电阻档位
 * @param  gear: GEAR_1A ~ GEAR_20UA (0~5)
 * @note   档位配置表:
 *         GEAR_1A:    5Ω,    1A,    5W,    引脚[RES~RES06] = 0000001
 *         GEAR_200MA: 25Ω,   200mA, 1W,    引脚[RES~RES06] = 0000010
 *         GEAR_20MA:  250Ω,  20mA,  100mW, 引脚[RES~RES06] = 0000100
 *         GEAR_2MA:   2.5KΩ, 2mA,   10mW,  引脚[RES~RES06] = 0001000
 *         GEAR_200UA: 25KΩ,  200uA, 1mW,   引脚[RES~RES06] = 0010000
 *         GEAR_20UA:  250KΩ, 20uA,  0.1mW, 引脚[RES~RES06] = 0100000
 * @retval HC_SMU_OK: 设置成功
 *         HC_SMU_ERR_INVALID_GEAR: 无效档位
 */
HC_SMU_Status_t HC_SMU_SetResistorGear(Resistor_Gear_t gear);

/**
 * @brief  获取当前负载电阻档位
 * @retval 当前档位 (GEAR_1A ~ GEAR_20UA)
 */
Resistor_Gear_t HC_SMU_GetResistorGear(void);

/**
 * @brief  关闭所有负载电阻
 * @note   用于模式切换或测量完成后断开所有负载
 * @retval HC_SMU_OK: 操作成功
 */
HC_SMU_Status_t HC_SMU_DisableAllResistors(void);

/*=============================================================================
 * 函数声明 - 综合控制函数
 *============================================================================*/

/**
 * @brief  配置完整的测量参数
 * @param  channel: 通道选择
 * @param  mode: 测量模式
 * @param  gear: 档位选择 (仅在电压源模式下有效)
 * @param  mos_state: MOS管状态
 * @note   此函数会按照正确顺序配置所有参数，避免配置冲突
 * @retval HC_SMU_OK: 配置成功
 *         HC_SMU_ERR_*: 对应的错误码
 */
HC_SMU_Status_t HC_SMU_ConfigMeasure(Channel_Select_t channel,
                                      Measure_Mode_t mode,
                                      Resistor_Gear_t gear,
                                      MOS_State_t mos_state);

/**
 * @brief  获取系统当前完整状态
 * @retval 指向系统状态结构体的指针
 */
HC_SMU_State_t* HC_SMU_GetSystemState(void);

/*=============================================================================
 * 函数声明 - 辅助调试函数
 *============================================================================*/

/**
 * @brief  获取档位描述字符串
 * @param  gear: 档位枚举
 * @retval 档位描述字符串指针
 */
const char* HC_SMU_GetGearDescription(Resistor_Gear_t gear);

/**
 * @brief  获取模式描述字符串
 * @param  mode: 模式枚举
 * @retval 模式描述字符串指针
 */
const char* HC_SMU_GetModeDescription(Measure_Mode_t mode);

#endif /* INC_HC_SMU_CONTROL_H_ */
