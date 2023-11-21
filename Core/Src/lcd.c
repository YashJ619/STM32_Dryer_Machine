/*
 * lcd.c
 *
 *  Created on: Mar 30, 2023
 *      Author: Yash
 */

#include "lcd.h"

uint8_t _displayfunction = 0;
uint8_t _displaycontrol = 0;
uint8_t _displaymode = 0;
uint8_t _entryset = 0;
uint8_t _shiftset = 0;
uint8_t _numlines = 0;
uint8_t _currline = 0;

void lcd_init(uint8_t fourbitmode)
{
	if (fourbitmode)
		_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	else
		_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	if (lines > 1)
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
	_currline = 0;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait
	// 50
	HAL_Delay(1000);
	// Now we pull both RS and R/W low to begin commands
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN, GPIO_PIN_RESET);


	// put the LCD into 4 bit or 8 bit mode
	//TODO (to be implemented)
	/*	if (!(_displayfunction & LCD_8BITMODE)) {
		// this is according to the hitachi HD44780 datasheet
	    // figure 24, pg 46

	    // we start in 8bit mode, try to set 4 bit mode
	    write4bits(0x03);
	    delayMicroseconds(4500); // wait min 4.1ms

	    // second try
	    write4bits(0x03);
	    delayMicroseconds(4500); // wait min 4.1ms

	    // third go!
	    write4bits(0x03);
	    delayMicroseconds(150);

	    // finally, set to 8-bit interface
	    write4bits(0x02);
	  } else {
		  */
	    // this is according to the hitachi HD44780 datasheet
	    // page 45 figure 23

	    // Send function set command sequence
		lcd_send_cmd(CMD_FUNCTIONSET | _displayfunction); // 38
		HAL_Delay(10); // wait more than 4.1ms

	    // second try
	    lcd_send_cmd(CMD_FUNCTIONSET | _displayfunction); //38
	    HAL_Delay(1);

	    // third go
	    lcd_send_cmd(CMD_FUNCTIONSET | _displayfunction); //38
	  //}

	// finally, set # lines, font size, etc.
	lcd_send_cmd(CMD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = DISPLAY_ON | CURSOR_OFF | BLINK_OFF;
	lcd_display(1);

	// clear it off
	lcd_clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDEC;

	// set the entry mode
	lcd_send_cmd(CMD_ENTRYMODE | _displaymode);
}

void lcd_home(void){
	lcd_send_cmd(CMD_RETURNHOME); // set cursor position to zero
	HAL_Delay(2); // this command takes a long time!
}

void lcd_display(uint8_t state)
{
	if(state)
	{
		_displaycontrol |= DISPLAY_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
	else
	{
		_displaycontrol &= ~DISPLAY_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
}

void lcd_enable(void)
{
	HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN, GPIO_PIN_RESET);
	for(int i = 0;i<5000;i++);
	HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN, GPIO_PIN_SET);
	for(int i = 0;i<5000;i++); // enable pulse must be >450ns
	HAL_GPIO_WritePin(LCD_EN_Port, LCD_EN, GPIO_PIN_RESET);
	for(int i = 0;i<5000;i++); // commands need > 37us to settle
}

void write_8bit(uint8_t value)
{
	HAL_GPIO_WritePin(LCD_D7_Port, LCD_D7, (value >> 7) & 0x01);
	HAL_GPIO_WritePin(LCD_D6_Port, LCD_D6, (value >> 6) & 0x01);
	HAL_GPIO_WritePin(LCD_D5_Port, LCD_D5, (value >> 5) & 0x01);
	HAL_GPIO_WritePin(LCD_D4_Port, LCD_D4, (value >> 4) & 0x01);
	HAL_GPIO_WritePin(LCD_D3_Port, LCD_D3, (value >> 3) & 0x01);
	HAL_GPIO_WritePin(LCD_D2_Port, LCD_D2, (value >> 2) & 0x01);
	HAL_GPIO_WritePin(LCD_D1_Port, LCD_D1, (value >> 1) & 0x01);
	HAL_GPIO_WritePin(LCD_D0_Port, LCD_D0, (value >> 0) & 0x01);

	lcd_enable();
}

void lcd_send_cmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS, GPIO_PIN_RESET);
	write_8bit(cmd);
}

void lcd_clear(void)
{
	lcd_send_cmd(CMD_CLEARDISPLAY);
	HAL_Delay(2);
}

void lcd_send_data(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS, GPIO_PIN_SET);
	write_8bit(data);
}

void lcd_cursor(uint8_t state)
{
	if(state)
	{
		_displaycontrol |= CURSOR_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
	else
	{
		_displaycontrol &= ~CURSOR_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
}

void lcd_blink(uint8_t state)
{
	if(state)
	{
		_displaycontrol |= BLINK_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
	else
	{
		_displaycontrol &= ~BLINK_ON;
		lcd_send_cmd(CMD_DISPLAYCONTROL | _displaycontrol);
	}
}

void lcd_scrollDisplayLeft(void)
{
	lcd_send_cmd(CMD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scrollDisplayRight(void)
{
	lcd_send_cmd(CMD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	lcd_send_cmd(CMD_ENTRYMODE | _displaymode);
}

void lcd_rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	lcd_send_cmd(CMD_ENTRYMODE | _displaymode);
}

void lcd_autoscroll(uint8_t state)
{
	if(state)
	{
		_displaymode |= LCD_ENTRYSHIFTINC;
		lcd_send_cmd(CMD_ENTRYMODE | _displaymode);
	}
	else
	{
		_displaymode &= ~LCD_ENTRYSHIFTINC;
		lcd_send_cmd(CMD_ENTRYMODE | _displaymode);
	}
}

void lcd_print(char *str)
{
	do
	{
		lcd_send_data((uint8_t)*str);
		str++;
	}while(*str != '\0');
}

void lcd_set_cursor(int8_t col, int8_t row)
{
	int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	if (row > _numlines) {
		row = _numlines - 1; // we count rows starting w/0
		}
	lcd_send_cmd(CMD_SETDDRAM_ADDR | (col + row_offsets[row]));
}

//void lcd_set_cursor(uint8_t row, uint8_t col)
//{
//	col--;
//	switch(row)
//	{
//	case 1:
//		lcd_send_cmd(col |= 0x80);
//		break;
//	case 2:
//		lcd_send_cmd(col |= 0xC0);
//		break;
//	case 3:
//		lcd_send_cmd(col |= 0x90);
//		break;
//	case 4:
//		lcd_send_cmd(col |= 0xD0);
//		break;
//	default:
//		break;
//	}
//}
