/*
 * output.c
 *
 *  Created on: Mar 27, 2023
 *      Author: Yash
 */
#include "output.h"

void output_off(void)
{
	HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
}
