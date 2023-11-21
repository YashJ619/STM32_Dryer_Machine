/*
 * keypad.h
 *
 *  Created on: Mar 26, 2023
 *      Author: Yash
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include"stm32f1xx_hal.h"
#include "main.h"

enum
{
	NO_KEY_PRESSED = 0,
	LOW_KEY_PRESSED,
	MED_KEY_PRESSED,
	HIGH_KEY_PRESSED,
	FN_KEY_PRESSED,
};

uint8_t scan_keypad(void);

#endif /* INC_KEYPAD_H_ */
