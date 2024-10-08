/*
 * output.h
 *
 *  Created on: Mar 27, 2023
 *      Author: Yash
 */

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

#include "stm32f1xx_hal.h"

#define DRUM_LEFT_PIN			GPIO_PIN_13
#define DRUM_RIGHT_PIN			GPIO_PIN_14
#define FAN_PIN					GPIO_PIN_8
#define HEATER_PIN				GPIO_PIN_15

#define BUZZER_PIN				GPIO_PIN_15
#define BUZZER_PORT				GPIOC

#define OUTPUT_PORT				GPIOB
#define FAN_PORT				GPIOA


void output_init(void);
void output_off(void);

#endif /* INC_OUTPUT_H_ */
