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

uint8_t is_door_open(void) {
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if (HAL_GetTick() - last_time > 5U) {
		if (HAL_GPIO_ReadPin(DOOR_SW_GPIO_Port, DOOR_SW_Pin) == GPIO_PIN_RESET) {
			last_state = 0;
		} else {
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return last_state;
}

uint8_t is_coil_open(void) {
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if (HAL_GetTick() - last_time > 5U) {
		if (HAL_GPIO_ReadPin(HEATER_SW_GPIO_Port, HEATER_SW_Pin) == GPIO_PIN_RESET) {
			last_state = 0;
		} else {
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return last_state;
}

uint8_t limit_sw_open(void) {
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if (HAL_GetTick() - last_time > 5U) {
		if (HAL_GPIO_ReadPin(LIMIT_SW_GPIO_Port, LIMIT_SW_Pin) == GPIO_PIN_RESET) {
			last_state = 0;
		} else {
			last_state = 1;
		}
		last_time = HAL_GetTick();
	}
	return last_state;
}

uint8_t sel_rot_pressed(void) {
	static volatile uint8_t last_state = 0U;
	static volatile uint32_t last_time = 0U;

	if (HAL_GetTick() - last_time > 5U) {
		if (HAL_GPIO_ReadPin(SEL_ROT_SW_GPIO_Port, SEL_ROT_SW_Pin) == GPIO_PIN_RESET) {
			last_state = 1;
		} else {
			last_state = 0;
		}
		last_time = HAL_GetTick();
	}
	return last_state;
}
/* USER CODE END 2 */
