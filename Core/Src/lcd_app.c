/*
 * lcd_app.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Yash
 */


#include "lcd_app.h"
#include "stdio.h"
#include "string.h"

extern MachineInit_t dryer;

char lcd_buf[80];

void welcome_page(void);
void door_open_page(void);
void heater_coil_page(void);
void mode_level_page(void);
void limit_sw_err_page(void);
void complete_page(void);

void lcd_update(void)
{
	switch(cur_page){
	case INIT_PAGE:
		welcome_page();
		break;
	case LOW_LEVEL_PAGE:
	case MED_LEVEL_PAGE:
	case HIGH_LEVEL_PAGE:
		mode_level_page();
		break;
	case COMPLETE_PAGE:
		complete_page();
		break;
	case DOOR_OPEN_PAGE:
		door_open_page();
		break;
	case LIMIT_SW_ERR_PAGE:
		limit_sw_err_page();
		break;
	case HEATER_COIL_PAGE:
		heater_coil_page();
		break;
	}

}

void welcome_page(void)
{
	lcd_clear();
	lcd_set_cursor(6, 0);
	lcd_print("WELCOME!");
	lcd_set_cursor(3,2);
	lcd_print("Select a mode");
	lcd_set_cursor(3,3);
	lcd_print("to start dryer");
}

void door_open_page(void)
{
	lcd_clear();
	lcd_set_cursor(6, 0);
	lcd_print("DOOR");
	lcd_set_cursor(2,2);
	lcd_print("OPEN");
}

void heater_coil_page(void)
{
	lcd_clear();
	lcd_set_cursor(5, 0);
	lcd_print("ERROR");
	lcd_set_cursor(1,2);
	lcd_print("HEATER");
}

void limit_sw_err_page(void)
{
	lcd_clear();
	lcd_set_cursor(5, 0);
	lcd_print("ERROR");
	lcd_set_cursor(2, 2);
	lcd_print("AIR");
}

void complete_page(void)
{
	lcd_clear();
	lcd_set_cursor(4, 0);
	lcd_print("PROCESS");
	lcd_set_cursor(0, 2);
	lcd_print("COMPLETE");
}
void mode_level_page(void)
{
	static uint16_t a = 0,b = 0;
	if(last_page!=cur_page)
	{
		lcd_clear();
		last_page = cur_page;
	}
	else lcd_home();
	a = dryer.heatTime % 60;
	b = dryer.heatTime / 60;
	sprintf(lcd_buf,"h=%02d:%02d",b,a);
	lcd_set_cursor(0, 0);
	lcd_print(lcd_buf);

	a = dryer.coolTime % 60;
	b = dryer.coolTime / 60;
	sprintf(lcd_buf,"c=%02d:%02d",b,a);
	lcd_set_cursor(13, 0);
	lcd_print(lcd_buf);

	if(dryer.mode == LOW_LEVEL)sprintf(lcd_buf,"LEVEL: LOW");
	else if(dryer.mode == MED_LEVEL)sprintf(lcd_buf,"LEVEL: MEDIUM");
	else if(dryer.mode == HIGH_LEVEL)sprintf(lcd_buf,"LEVEL: HIGH");
	else;

	lcd_set_cursor(5, 1);
	lcd_print(lcd_buf);

	sprintf(lcd_buf,"Set:%02dC",dryer.setTemp);
	lcd_set_cursor(0, 3);
	lcd_print(lcd_buf);

	sprintf(lcd_buf,"Cur:ERR");
	lcd_set_cursor(13, 3);
	lcd_print(lcd_buf);
}
