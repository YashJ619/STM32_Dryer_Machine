/*
 * keypad.c
 *
 *  Created on: Mar 26, 2023
 *      Author: Yash
 */
#include "keypad.h"

uint8_t is_low_key_pressed(void);
uint8_t is_med_key_pressed(void);
uint8_t is_high_key_pressed(void);
uint8_t is_fn_key_pressed(void);

void keypad_init(void)
{
	 __HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIOx = {0};

	GPIOx.Pin = LOW_KEY | MED_KEY | HIGH_KEY | FN_KEY;
	GPIOx.Mode = GPIO_MODE_INPUT;
	GPIOx.Pull = GPIO_PULLUP;
	GPIOx.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(KEYPAD_PORT, &GPIOx);
}

uint8_t scan_keypad(void)
{
	if(is_low_key_pressed())
	{
		printf("LOW KEY PRESSED\r\n");
		return LOW_KEY_PRESSED;
	}
	else if(is_med_key_pressed())
	{
		printf("MED KEY PRESSED\r\n");
		return MED_KEY_PRESSED;
	}
	else if(is_high_key_pressed())
	{
		printf("HIGH KEY PRESSED\r\n");
		return HIGH_KEY_PRESSED;
	}
	else if(is_fn_key_pressed())
	{
		printf("FN KEY PRESSED\r\n");
		return FN_KEY_PRESSED;
	}
	return NO_KEY_PRESSED;
}

uint8_t is_low_key_pressed()
{
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if(HAL_GetTick() - last_time > 10U)
	{
		if(HAL_GPIO_ReadPin(KEYPAD_PORT, LOW_KEY) == GPIO_PIN_RESET)
		{
			if(last_state == 1)
			{
				if(HAL_GPIO_ReadPin(KEYPAD_PORT, LOW_KEY) == GPIO_PIN_RESET)
				{
					last_state = 0;
					return 1;
				}
				else
				{
					last_state = 1;
				}
			}
		}
		else
		{
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return 0;
}

uint8_t is_med_key_pressed()
{
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if(HAL_GetTick() - last_time > 10U)
	{
		if(HAL_GPIO_ReadPin(KEYPAD_PORT, MED_KEY) == GPIO_PIN_RESET)
		{
			if(last_state == 1)
			{
				if(HAL_GPIO_ReadPin(KEYPAD_PORT, MED_KEY) == GPIO_PIN_RESET)
				{
					last_state = 0;
					return 1;
				}
				else
				{
					last_state = 1;
				}
			}
		}
		else
		{
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return 0;
}

uint8_t is_high_key_pressed()
{
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if(HAL_GetTick() - last_time > 10U)
	{
		if(HAL_GPIO_ReadPin(KEYPAD_PORT, HIGH_KEY) == GPIO_PIN_RESET)
		{
			if(last_state == 1)
			{
				if(HAL_GPIO_ReadPin(KEYPAD_PORT, HIGH_KEY) == GPIO_PIN_RESET)
				{
					last_state = 0;
					return 1;
				}
				else
				{
					last_state = 1;
				}
			}
		}
		else
		{
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return 0;
}

uint8_t is_fn_key_pressed()
{
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if(HAL_GetTick() - last_time > 10U)
	{
		if(HAL_GPIO_ReadPin(KEYPAD_PORT, FN_KEY) == GPIO_PIN_RESET)
		{
			if(last_state == 1)
			{
				if(HAL_GPIO_ReadPin(KEYPAD_PORT, FN_KEY) == GPIO_PIN_RESET)
				{
					last_state = 0;
					return 1;
				}
				else
				{
					last_state = 1;
				}
			}
		}
		else
		{
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return 0;
}
