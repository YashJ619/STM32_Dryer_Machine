/*
 * inputs.h
 *
 *  Created on: Mar 27, 2023
 *      Author: Yash
 */
#ifndef __INPUTS_H__
#define __INPUTS_H__

#include "stm32f1xx_hal.h"
#include "main.h"

#define INPUT_PORT			GPIOA

uint8_t is_door_open(void);
uint8_t is_coil_open(void);
uint8_t limit_sw_open(void);
uint8_t sel_rot_pressed(void);

#endif /*__ GPIO_H__ */

