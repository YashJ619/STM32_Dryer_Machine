/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
void input_init(void);
void keypad_init(void);
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  input_init();
  keypad_init();
}

/* USER CODE BEGIN 2 */
void input_init(void)
{

	GPIO_InitTypeDef GPIOx = {0};

	GPIOx.Pin = DOOR_SW;
	GPIOx.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIOx.Pull = GPIO_PULLUP;
	GPIOx.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(INPUT_PORT, &GPIOx);

	GPIOx.Pin = LIMIT_SW | HEATER_SW | SEL_ROT_SW;
	GPIOx.Mode = GPIO_MODE_INPUT;
	GPIOx.Pull = GPIO_PULLUP;
	GPIOx.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(INPUT_PORT, &GPIOx);

	GPIOx.Pin = LED;
	GPIOx.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOx.Pull = GPIO_NOPULL;
	GPIOx.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC, &GPIOx);

	//HAL_GPIO_WritePin(GPIOC, LED, LOW);

	HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(DOOR_SW);
}

void keypad_init(void)
{
	GPIO_InitTypeDef GPIOx = {0};

	GPIOx.Pin = LOW_KEY | MED_KEY | HIGH_KEY | FN_KEY;
	GPIOx.Mode = GPIO_MODE_INPUT;
	GPIOx.Pull = GPIO_PULLUP;
	GPIOx.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(KEYPAD_PORT, &GPIOx);
}
/* USER CODE END 2 */
