/*
 * lcd_app.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Yash
 */

#ifndef INC_LCD_APP_H_
#define INC_LCD_APP_H_

#include "lcd.h"
#include "machine.h"

typedef enum{
		INIT_PAGE,
		LOW_LEVEL_PAGE,
		MED_LEVEL_PAGE,
		HIGH_LEVEL_PAGE,
		COMPLETE_PAGE,
		DOOR_OPEN_PAGE,
		LIMIT_SW_ERR_PAGE,
		HEATER_COIL_PAGE,
}LCDPageTypeDef;

extern LCDPageTypeDef cur_page;
extern LCDPageTypeDef last_page;


void lcd_update(void);

#endif /* INC_LCD_APP_H_ */
