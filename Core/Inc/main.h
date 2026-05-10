/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define VERSION0   	0x2025
#define VERSION1	0x1220
#define VERSION2  	0x1905

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// 保持寄存�?
#pragma pack(push, 1)  // 强制1字节对齐
typedef struct HOLD_REG
{
	uint16_t SYS_Lock0;					// [0] 系统锁止寄存器0

	uint16_t SYS_Lock1;					// [1] 系统锁止寄存器1

	uint16_t SYS_Switch_LoadRes;  		// [2] 切换负载电阻2

    uint16_t SYS_Switch_channle; 		// [3] 切换通道3

    uint16_t SYS_Switch_IVSource; 			// [4] 切换电流源，电压源4

    uint16_t SYS_Control_Shortground; 	// [5] 控制电流源和电压源输出短接到地5

}HOLD_REG;
#pragma pack(pop)  // 恢复默认对齐

// 输入寄存�?
#pragma pack(push, 1)  // 强制1字节对齐
typedef struct INPUT_REG
{
	uint16_t Version_0;		// [0] 固件版本寄存�?0，年

	uint16_t Version_1;		// [1] 固件版本寄存�?1，月�?

	uint16_t Version_2;		// [2] 固件版本寄存�?2，时�?

}INPUT_REG;
#pragma pack(pop)  // 恢复默认对齐

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern HOLD_REG* volatile sys_set;
extern INPUT_REG* volatile sys_dat;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RELAY_RES01_Pin GPIO_PIN_1
#define RELAY_RES01_GPIO_Port GPIOC
#define RELAY_RES02_Pin GPIO_PIN_2
#define RELAY_RES02_GPIO_Port GPIOC
#define RELAY_RES03_Pin GPIO_PIN_3
#define RELAY_RES03_GPIO_Port GPIOC
#define RELAY_RES_Pin GPIO_PIN_1
#define RELAY_RES_GPIO_Port GPIOA
#define RELAY_OS_Pin GPIO_PIN_2
#define RELAY_OS_GPIO_Port GPIOA
#define MOS_Pin GPIO_PIN_3
#define MOS_GPIO_Port GPIOA
#define RELAY_CH01_Pin GPIO_PIN_4
#define RELAY_CH01_GPIO_Port GPIOA
#define RELAY_CH02_Pin GPIO_PIN_5
#define RELAY_CH02_GPIO_Port GPIOA
#define RELAY_CH03_Pin GPIO_PIN_6
#define RELAY_CH03_GPIO_Port GPIOA
#define RELAY_CH04_Pin GPIO_PIN_7
#define RELAY_CH04_GPIO_Port GPIOA
#define RELAY_RES04_Pin GPIO_PIN_4
#define RELAY_RES04_GPIO_Port GPIOC
#define RELAY_RES05_Pin GPIO_PIN_5
#define RELAY_RES05_GPIO_Port GPIOC
#define RELAY_RES06_Pin GPIO_PIN_0
#define RELAY_RES06_GPIO_Port GPIOB
#define USART1_CTS_Pin GPIO_PIN_11
#define USART1_CTS_GPIO_Port GPIOA
#define USART1_RTS_Pin GPIO_PIN_12
#define USART1_RTS_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
