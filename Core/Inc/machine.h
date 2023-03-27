/*
 * machine.h
 *
 *  Created on: Mar 26, 2023
 *      Author: Yash
 */

#ifndef INC_MACHINE_H_
#define INC_MACHINE_H_

#include "stm32f1xx_hal.h"
#include "keypad.h"
#include "output.h"

#define HIGH 			GPIO_PIN_SET
#define LOW 			GPIO_PIN_RESET

typedef enum
{
	INIT,
	START,
	STOP
}StateTypeDef_t;

typedef enum
{
	NO_MODE,
	LOW_LEVEL,
	MED_LEVEL,
	HIGH_LEVEL
}ModeTypeDef_t;

enum
{
	HEAT_CYCLE,
	COOL_CYCLE,
};

typedef struct
{
	StateTypeDef_t state;
	ModeTypeDef_t mode;
	uint16_t heatTime;
	uint16_t coolTime;
	uint8_t cycle;
}MachineInit_t;

extern MachineInit_t dryer;

#endif /* INC_MACHINE_H_ */
