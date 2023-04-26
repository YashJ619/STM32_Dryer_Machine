/*
 * inputs.h
 *
 *  Created on: Mar 27, 2023
 *      Author: Yash
 */
#ifndef __INPUTS_H__
#define __INPUTS_H__

#include "stm32f1xx_hal.h"

#define LED					GPIO_PIN_13
#define DOOR_SW 			GPIO_PIN_0
#define LIMIT_SW 			GPIO_PIN_1
#define SEL_ROT_SW 			GPIO_PIN_2
#define HEATER_SW 			GPIO_PIN_3

#define INPUT_PORT			GPIOA

void input_init(void);
uint8_t is_door_open(void);
uint8_t is_coil_open(void);

#endif /*__ GPIO_H__ */

