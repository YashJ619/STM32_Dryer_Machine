/*
 * keypad.c
 *
 *  Created on: Mar 26, 2023
 *      Author: Yash
 */
#include "keypad.h"

uint8_t is_key_pressed(uint16_t Pin);

void keypad_init(void)
{
	 __HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIOx = {0};

	GPIOx.Pin = LOW_KEY | MED_KEY | HIGH_KEY | FN_KEY;
	GPIOx.Mode = GPIO_MODE_INPUT;
	GPIOx.Pull = GPIO_PULLUP;
	GPIOx.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(KEYPAD_PORT, &GPIOx);
}

uint8_t scan_keypad(void)
{
	if(is_key_pressed(LOW_KEY))
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)"LOW KEY PRESSED\r\n", sizeof("LOW KEY PRESSED\r\n"), 10);
		return LOW_KEY_PRESSED;
	}
	else if(is_key_pressed(MED_KEY))
	{
	  HAL_UART_Transmit(&huart1, (uint8_t*)"MED KEY PRESSED\r\n", sizeof("MED KEY PRESSED\r\n"), 10);
	  return MED_KEY_PRESSED;
	}
	else if(is_key_pressed(HIGH_KEY))
	{
	  HAL_UART_Transmit(&huart1, (uint8_t*)"HIGH KEY PRESSED\r\n", sizeof("HIGH KEY PRESSED\r\n"), 10);
	  return HIGH_KEY_PRESSED;
	}
	else if(is_key_pressed(FN_KEY))
	{
	  HAL_UART_Transmit(&huart1, (uint8_t*)"FN KEY PRESSED\r\n", sizeof("FN KEY PRESSED\r\n"), 10);
	  return FN_KEY_PRESSED;
	}
	return NO_KEY_PRESSED;
}

uint8_t is_key_pressed(uint16_t Pin)
{
	static uint8_t temp = 0;
	temp = HAL_GPIO_ReadPin(KEYPAD_PORT, Pin);
	return !temp;
}
