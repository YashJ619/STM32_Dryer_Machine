/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOC
#define DOOR_SW_Pin GPIO_PIN_0
#define DOOR_SW_GPIO_Port GPIOA
#define DOOR_SW_EXTI_IRQn EXTI0_IRQn
#define LIMIT_SW_Pin GPIO_PIN_1
#define LIMIT_SW_GPIO_Port GPIOA
#define SEL_ROT_SW_Pin GPIO_PIN_2
#define SEL_ROT_SW_GPIO_Port GPIOA
#define HEATER_SW_Pin GPIO_PIN_3
#define HEATER_SW_GPIO_Port GPIOA
#define SS_Pin GPIO_PIN_4
#define SS_GPIO_Port GPIOA
#define FN_KEY_Pin GPIO_PIN_0
#define FN_KEY_GPIO_Port GPIOB
#define LOW_KEY_Pin GPIO_PIN_1
#define LOW_KEY_GPIO_Port GPIOB
#define MED_KEY_Pin GPIO_PIN_10
#define MED_KEY_GPIO_Port GPIOB
#define HIGH_KEY_Pin GPIO_PIN_11
#define HIGH_KEY_GPIO_Port GPIOB
#define DRUM_LEFT_Pin GPIO_PIN_13
#define DRUM_LEFT_GPIO_Port GPIOB
#define DRUM_RIGHT_Pin GPIO_PIN_14
#define DRUM_RIGHT_GPIO_Port GPIOB
#define HEATER_Pin GPIO_PIN_15
#define HEATER_GPIO_Port GPIOB
#define FAN_Pin GPIO_PIN_8
#define FAN_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_11
#define LCD_RS_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_12
#define LCD_EN_GPIO_Port GPIOA
#define LCD_D0_Pin GPIO_PIN_15
#define LCD_D0_GPIO_Port GPIOA
#define LCD_D1_Pin GPIO_PIN_3
#define LCD_D1_GPIO_Port GPIOB
#define LCD_D2_Pin GPIO_PIN_4
#define LCD_D2_GPIO_Port GPIOB
#define LCD_D3_Pin GPIO_PIN_5
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_6
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_7
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_8
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_9
#define LCD_D7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
