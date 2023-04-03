/*
 * lcd.h
 *
 *  Created on: Mar 30, 2023
 *      Author: Yash
 */
#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f1xx_hal.h"

#define LCD_PIN_RS				GPIO_PIN_9
#define LCD_PIN_EN				GPIO_PIN_8
#define LCD_PIN_D0				GPIO_PIN_7
#define LCD_PIN_D1				GPIO_PIN_6
#define LCD_PIN_D2				GPIO_PIN_5
#define LCD_PIN_D3				GPIO_PIN_4
#define LCD_PIN_D4				GPIO_PIN_3
#define LCD_PIN_D5				GPIO_PIN_15
#define LCD_PIN_D6				GPIO_PIN_12
#define LCD_PIN_D7				GPIO_PIN_11

#define LCD_PORT_CTRL			GPIOB
#define LCD_PORT_D0_4			GPIOB
#define LCD_PORT_D5_7			GPIOA

/********************************************
 * COMMANDS
 *******************************************/
#define	CMD_CLEARDISPLAY    	0x01U //<Clear display, set cursor position to zero>
#define CMD_RETURNHOME      	0x02U //<Set cursor position to zero>
#define CMD_ENTRYMODE			0x04U //<Sets the entry mode>
#define CMD_DISPLAYCONTROL	    0x08U //<Controls the display; does stuff like turning it off and on>
#define CMD_CURSORSHIFT   		0x10U //<Lets you move the cursor>
#define CMD_FUNCTIONSET			0x20U //<Used to send the function to set to the display>
#define CMD_SETCGRAM_ADDR      	0x40U //<Used to set the CGRAM (character generator RAM) with characters>
#define CMD_SETDDRAM_ADDR      	0x80U //<Used to set the DDRAM (Display Data RAM)>

/*******************************************
 * flags for display entry mode
 *******************************************/
#define LCD_ENTRYRIGHT 			0x00U //<Used to set text to flow from right to left>
#define LCD_ENTRYLEFT 			0x02U //<Used to set text to flow from left to right>
#define LCD_ENTRYSHIFTINC		0X01U //<Used to 'right justify' text from the cursor>
#define LCD_ENTRYSHIFTDEC		0X00 //<Used to 'left justify' text from the cursor>

/*******************************************
 * flags for display on/off control
 ******************************************/
#define DISPLAY_ON	    		  0x04U //<Turns the display on>
#define DISPLAY_OFF	    		  0X00U //<Turns the display off>
#define CURSOR_ON     			  0x02U //<Turns the cursor on>
#define CURSOR_OFF     			  0x00U //<Turns the cursor off>
#define BLINK_ON      			  0x01U //<Turns on the blinking cursor>
#define BLINK_OFF      			  0X00U //<Turns off the blinking cursor>

/******************************************
 * flags for display/cursor shift
 *****************************************/
#define LCD_DISPLAYMOVE			  0x08U //<Flag for moving the display>
#define LCD_CURSORMOVE 			  0x00U //<Flag for moving the cursor>
#define LCD_MOVERIGHT 			  0x04U //<Flag for moving right>
#define LCD_MOVELEFT 			  0x00U //<Flag for moving left>

/****************************************
 * flags for function set
 ***************************************/
#define LCD_8BITMODE 			  0x10U //<LCD 8 bit mode>
#define LCD_4BITMODE 			  0x00U //<LCD 4 bit mode>
#define LCD_2LINE 				  0x08U //<LCD 2 line mode>
#define LCD_1LINE 				  0x00U //<LCD 1 line mode>
#define LCD_5x10DOTS 			  0x04U //<10 pixel high font mode>
#define LCD_5x8DOTS   			  0x00U  //<8 pixel high font mode>

void lcd_gpio_init(void);
void lcd_init(uint8_t fourbitmode);
void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
void lcd_home(void);
void lcd_display(uint8_t state);
void lcd_enable(void);
void write_8bit(uint8_t value);
void lcd_send_cmd(uint8_t cmd);
void lcd_clear(void);
void lcd_send_data(uint8_t data);
void lcd_cursor(uint8_t state);
void lcd_blink(uint8_t state);
void lcd_scrollDisplayLeft(void);
void lcd_scrollDisplayRight(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(uint8_t state);
void lcd_print(char *str);
void lcd_set_cursor(uint8_t col, uint8_t row);

#endif /* INC_LCD_H_ */
