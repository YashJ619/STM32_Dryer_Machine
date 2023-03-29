/*
 * timer.h
 *
 *  Created on: Mar 29, 2023
 *      Author: Yash
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim4;

void timer_init(void);

#endif /* INC_TIMER_H_ */
