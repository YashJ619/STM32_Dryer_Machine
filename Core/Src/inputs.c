/*
 * inputs.c
 *
 *  Created on: Mar 27, 2023
 *      Author: Yash
 */
#include <inputs.h>

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void input_init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

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

uint8_t door_open(void)
{
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if(HAL_GetTick() - last_time > 5U)
	{
		if(HAL_GPIO_ReadPin(INPUT_PORT, DOOR_SW) == GPIO_PIN_RESET)
		{
			if(last_state == 1)
			{
				if(HAL_GPIO_ReadPin(INPUT_PORT, DOOR_SW) == GPIO_PIN_RESET)
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

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(DOOR_SW);
}
/* USER CODE END 2 */
