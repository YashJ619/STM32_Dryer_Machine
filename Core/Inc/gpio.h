/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define HIGH 			GPIO_PIN_SET
#define LOW 			GPIO_PIN_RESET

#define LED				GPIO_PIN_13


#define DOOR_SW 			GPIO_PIN_0
#define LIMIT_SW 			GPIO_PIN_1
#define SEL_ROT_SW 			GPIO_PIN_2
#define HEATER_SW 			GPIO_PIN_3

#define INPUT_PORT			GPIOA

#define LOW_KEY				GPIO_PIN_1
#define MED_KEY				GPIO_PIN_11
#define HIGH_KEY			GPIO_PIN_10
#define FN_KEY				GPIO_PIN_0

#define KEYPAD_PORT			GPIOB



/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

