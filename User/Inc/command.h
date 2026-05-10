/*
 * command.h
 *
 *  Created on: Dec 27, 2025
 *      Author: zzh22
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include "main.h"



#define UNLOCkREG0   	0xDEAD
#define UNLOCkREG1 		0xBEEF

// 系统锁止寄存器
#define CMD_LOCK_OFFSET 0
#define CMD_LOCK_LENGTH 2

// 负载电阻选择寄存器
#define CMD_RESISTOR_OFFSET 2
#define CMD_RESISTOR_LENGTH 1

// 通道选择寄存器
#define CMD_CHN_OFFSET 3
#define CMD_CHN_LENGTH 1

// 电流电压源选择寄存器
#define CMD_IV_OFFSET 4
#define CMD_IV_LENGTH 1

// 控制电流源和电压源输出短接到地寄存器
#define CMD_IVSHORT_OFFSET 5
#define CMD_IVSHORT_LENGTH 1

#define CMD_BOOTLOADER_SWITCH_OFFSET 255
#define CMD_BOOTLOADER_SWITCH_LENGTH  1

#define CMD_MOBUS_HOLD_REG_LENGTH (CMD_BOOTLOADER_SWITCH_OFFSET + CMD_BOOTLOADER_SWITCH_LENGTH)

#define CMD_BOARD_MON_OFFSET 32
#define CMD_BOARD_MON_LENGTH 8


#define CMD_MOBUS_INPUT_REG_LENGTH (CMD_BOARD_MON_OFFSET + CMD_BOARD_MON_LENGTH)

enum Lock_State{
	False = 0,
   True = 1,
};


/*=============================================================================
 * 功能函数声明
 *============================================================================*/

/**
 * @brief  初始化数据结构
 * @param  sys_dat: 输入寄存器结构体指针
 */
void init_DATA_Struct(INPUT_REG *sys_dat);

/**
 * @brief  命令处理主函数
 * @note   在主循环中调用，用于检测Modbus寄存器变化并执行相应控制
 */
void CMD_ProcessTask(void);

/**
 * @brief  检查系统锁定状态
 * @retval 1: 系统解锁, 0: 系统锁定
 */
uint8_t CMD_IsSystemUnlocked(void);


#endif /* INC_COMMAND_H_ */
