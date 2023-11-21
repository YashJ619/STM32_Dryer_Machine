/*************************************************************************************
 Title	 :  MAXIM Integrated MAX6675 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef INC_MAX6675_H_
#define INC_MAX6675_H_

#include "stm32f1xx_hal.h"
#include "main.h"

// ------------------------- Defines -------------------------
#define SSPORT SS_GPIO_Port       // GPIO Port of Chip Select(Slave Select)
#define SSPIN  SS_Pin  // GPIO PIN of Chip Select(Slave Select)
// ------------------------- Functions  ----------------------
float Max6675_Read_Temp(void);
#endif
