/*
 * command.c
 *
 *  Created on: Dec 27, 2025
 *      Author: zzh22
 *  Updated on: Jan 03, 2026
 *      Updated by: Claude Code
 *  Description: Modbus命令处理模块，负责解析寄存器指令并调用HC-SMU控制模块
 */
#include "command.h"
#include "usart.h"
#include "hc_smu_control.h"

uint16_t cmd_HoldReg_buf_au16[CMD_MOBUS_HOLD_REG_LENGTH] = {0};
uint16_t cmd_HoldReg_buf_old_au16[CMD_MOBUS_HOLD_REG_LENGTH] = {0};

/*=============================================================================
 * 私有函数声明
 *============================================================================*/
static uint8_t cmd_cache_holdreg(uint16_t offset, uint16_t length);
static uint8_t cmd_cmp_holdreg(uint16_t offset, uint16_t length);
static void cmd_process_resistor_change(void);
static void cmd_process_channel_change(void);
static void cmd_process_iv_source_change(void);
static void cmd_process_shortground_change(void);

/*=============================================================================
 * 私有函数实现 - 寄存器缓存与比较
 *============================================================================*/

static uint8_t cmd_cache_holdreg(uint16_t offset, uint16_t length)
{
    uint8_t ret = 0;
    uint16_t i;
    uint16_t index;
    if (offset + length < CMD_MOBUS_HOLD_REG_LENGTH)
    {
        for (i = 0; i < length; i++)
        {
            index = i + offset;
            cmd_HoldReg_buf_old_au16[index] = cmd_HoldReg_buf_au16[index];
        }
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

#define CMD_EQUAL 0
#define CMD_N_EQUAL 1

static uint8_t cmd_cmp_holdreg(uint16_t offset, uint16_t length)
{
    uint8_t ret = CMD_EQUAL;
    uint16_t i;
    uint16_t index;
    if (offset + length < CMD_MOBUS_HOLD_REG_LENGTH)
    {
        for (i = 0; i < length; i++)
        {
            index = i + offset;
            if (cmd_HoldReg_buf_old_au16[index] != cmd_HoldReg_buf_au16[index])
            {
                break;
            }
            else
            {
            }
        }
        if (i == length)
        {
            ret = CMD_EQUAL;
        }
        else
        {
            ret = CMD_N_EQUAL;
        }
    }
    else
    {
        ret = CMD_EQUAL;
    }
    return ret;
}


void init_DATA_Struct(INPUT_REG *sys_dat)
{
	sys_dat->Version_0 = VERSION0;
	sys_dat->Version_1 = VERSION1;
	sys_dat->Version_2 = VERSION2;
}

/*=============================================================================
 * 公共函数实现 - 系统锁定检查
 *============================================================================*/

/**
 * @brief  检查系统锁定状态
 * @retval 1: 系统解锁, 0: 系统锁定
 */
uint8_t CMD_IsSystemUnlocked(void)
{
    extern HOLD_REG* volatile sys_set;

    // 检查解锁寄存器是否正确
    if (sys_set->SYS_Lock0 == UNLOCkREG0 && sys_set->SYS_Lock1 == UNLOCkREG1) {
        return 1;
    }
    return 0;
}

/*=============================================================================
 * 公共函数实现 - 命令处理主函数
 *============================================================================*/

/**
 * @brief  命令处理主函数
 * @note   检测Modbus寄存器变化并执行相应控制
 */
void CMD_ProcessTask(void)
{
    extern HOLD_REG* volatile sys_set;

    // 系统必须先解锁才能执行命令
    if (!CMD_IsSystemUnlocked()) {
        return;
    }

    // 检测负载电阻寄存器变化
    if (cmd_cmp_holdreg(CMD_RESISTOR_OFFSET, CMD_RESISTOR_LENGTH) == CMD_N_EQUAL) {
        cmd_process_resistor_change();
        cmd_cache_holdreg(CMD_RESISTOR_OFFSET, CMD_RESISTOR_LENGTH);
    }

    // 检测通道选择寄存器变化
    if (cmd_cmp_holdreg(CMD_CHN_OFFSET, CMD_CHN_LENGTH) == CMD_N_EQUAL) {
        cmd_process_channel_change();
        cmd_cache_holdreg(CMD_CHN_OFFSET, CMD_CHN_LENGTH);
    }

    // 检测电流/电压源选择寄存器变化
    if (cmd_cmp_holdreg(CMD_IV_OFFSET, CMD_IV_LENGTH) == CMD_N_EQUAL) {
        cmd_process_iv_source_change();
        cmd_cache_holdreg(CMD_IV_OFFSET, CMD_IV_LENGTH);
    }

    // 检测短接到地寄存器变化
    if (cmd_cmp_holdreg(CMD_IVSHORT_OFFSET, CMD_IVSHORT_LENGTH) == CMD_N_EQUAL) {
        cmd_process_shortground_change();
        cmd_cache_holdreg(CMD_IVSHORT_OFFSET, CMD_IVSHORT_LENGTH);
    }
}

/*=============================================================================
 * 私有函数实现 - 各寄存器变化处理
 *============================================================================*/

/**
 * @brief  处理负载电阻寄存器变化
 */
static void cmd_process_resistor_change(void)
{
    extern HOLD_REG* volatile sys_set;
    uint16_t gear_value = sys_set->SYS_Switch_LoadRes;

    // 寄存器值范围: 0~5对应GEAR_1A~GEAR_20UA
    // 其他值: 关闭所有负载
    if (gear_value < RESISTOR_GEAR_COUNT) {
        HC_SMU_SetResistorGear((Resistor_Gear_t)gear_value);
    } else {
        HC_SMU_DisableAllResistors();
    }
}

/**
 * @brief  处理通道选择寄存器变化
 */
static void cmd_process_channel_change(void)
{
    extern HOLD_REG* volatile sys_set;
    uint16_t channel_value = sys_set->SYS_Switch_channle;

    // 寄存器值范围: 0~3对应CHANNEL_1~CHANNEL_4
    // 其他值: 关闭所有通道
    if (channel_value < CHANNEL_COUNT) {
        HC_SMU_SelectChannel((Channel_Select_t)channel_value);
    } else {
        HC_SMU_CloseAllChannels();
    }
}

/**
 * @brief  处理电流/电压源选择寄存器变化
 */
static void cmd_process_iv_source_change(void)
{
    extern HOLD_REG* volatile sys_set;
    uint16_t mode_value = sys_set->SYS_Switch_IVSource;

    // 寄存器值: 0=电流源模式, 1=电压源模式
    if (mode_value == 0) {
        HC_SMU_SetMeasureMode(MODE_CURRENT_SOURCE);
    } else if (mode_value == 1) {
        HC_SMU_SetMeasureMode(MODE_VOLTAGE_SOURCE);
    }
}

/**
 * @brief  处理短接到地寄存器变化
 */
static void cmd_process_shortground_change(void)
{
    extern HOLD_REG* volatile sys_set;
    uint16_t short_value = sys_set->SYS_Control_Shortground;

    // 寄存器值: 0=MOS关闭(HF停止工作), 1=MOS开启(HF正常工作)
    if (short_value == 0) {
        HC_SMU_SetMOSState(MOS_OFF);
    } else if (short_value == 1) {
        HC_SMU_SetMOSState(MOS_ON);
    }
}
