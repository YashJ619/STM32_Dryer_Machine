/*************************************************************************************
 Title	 :  MAXIM Integrated MAX6675 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
 *************************************************************************************/
#include"MAX6675.h"
extern SPI_HandleTypeDef hspi1;

// ------------------- Variables ----------------
_Bool TCF = 0;                       // Thermocouple Connection acknowledge Flag
uint8_t DATARX[2];                                    // Raw Data from MAX6675

// ------------------- Functions ----------------
float Max6675_Read_Temp(void) {
	float Temp = 0;                                      // Temperature Variable
	HAL_GPIO_WritePin(SSPORT, SSPIN, GPIO_PIN_RESET); // Low State for SPI Communication
	HAL_SPI_Receive(&hspi1, DATARX, 2, 50);                  // DATA Transfer
	HAL_GPIO_WritePin(SSPORT, SSPIN, GPIO_PIN_SET); // High State for SPI Communication
//printf("Mix = %x\r\n",((DATARX[0] << 8)|DATARX[1]));
//printf("MSB = %x\r\n",DATARX[1]);

//TCF=(((DATARX[0]|(DATARX[1]<<8))>>2) & 0x0001);        // State of Connecting
	TCF = ((((DATARX[0] << 8) | DATARX[1]) >> 2) & 0x0001);
//printf("TCF = %d\r\n",TCF);

//Temp=((((DATARX[0]|DATARX[1]<<8)))>>3);               // Temperature Data Extraction
	if (TCF == 0) {
		Temp = (((DATARX[0] << 8) | DATARX[1]) >> 3);
		Temp *= 0.25;                         // Data to Centigrade Conversation
	} else
		Temp = -1;
	//HAL_Delay(250); // Waits for Chip Ready(according to Datasheet, the max time for conversion is 220ms)
	return Temp;
}
