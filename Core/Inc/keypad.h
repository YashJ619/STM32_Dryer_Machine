/*
 * keypad.h
 *
 *  Created on: Mar 26, 2023
 *      Author: Yash
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include"stm32f1xx_hal.h"
#include "usart.h"

#define LOW_KEY				GPIO_PIN_1
#define MED_KEY				GPIO_PIN_11
#define HIGH_KEY			GPIO_PIN_10
#define FN_KEY				GPIO_PIN_0

#define KEYPAD_PORT			GPIOB

enum
{
	NO_KEY_PRESSED = 0,
	LOW_KEY_PRESSED,
	MED_KEY_PRESSED,
	HIGH_KEY_PRESSED,
	FN_KEY_PRESSED,
};

uint8_t scan_keypad(void);
void keypad_init(void);

#endif /* INC_KEYPAD_H_ */
