/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "machine.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
MachineInit_t dryer;

uint8_t door_open_flag = 0U; //Indicates Door open or close (1 for open, 0 for close)
uint8_t door_open_evt = 0U; //to excecute door open routine once

uint8_t timer_stop_evt = 0U; //to stop timer routine once
uint8_t timer_start_evt = 0U; //to start timer routine once

uint8_t buzzer_evt = 0U; //to excecute buzzer routine once

uint8_t lcd_update_evt = 0U; //to excecute LCD Update routine once

uint8_t op_update_evt = 0U; //to excecute Output Update routine once

uint8_t limit_timeout = 30U; //to store limit switch timeout default value
uint8_t limit_timeout_flag = 0U; //Indicates Limit Switch Timeout

uint8_t coil_open_flag = 0U; //Indicates Heater Coil open or close (1 for open, 0 for close)

uint8_t rot_left_cunt = 30U; //to store left side count value
uint8_t rot_right_cunt = 0U; //to store right side count value
uint8_t pasue_cunt = 0U; //to store delay count value for flip-flop

uint8_t rot_sw_state = 0U; //to store Rotation Select Switch
uint8_t flipflop = 0U; //to store Flip-Flop state 0 for L-to-R, 1 for R-to-L

int cur_temp = -1; //to store current temperature value
uint8_t otp = 1; //not in use
uint8_t scaned_key = NO_KEY_PRESSED; //to store any key pressed value

LCDPageTypeDef cur_page; //Current Page of LCD
LCDPageTypeDef last_page; //Store Last Page Recored

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*For Debug use printf to Print data on UART*/
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 10);
	return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

	lcd_init(0);
	lcd_begin(16, 4, 0);

	dryer.state = INIT;
	dryer.mode = NO_MODE;

	lcd_logo();

	HAL_Delay(3000);

	lcd_clear();


	if (HAL_GPIO_ReadPin(DOOR_SW_GPIO_Port, DOOR_SW_Pin) == HIGH) {
		door_open_flag = 1U;
		cur_page = DOOR_OPEN_PAGE;
	}
	else{
		cur_page = INIT_PAGE;
	}

	lcd_update();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/**********************************************************************************************************
		 **************************************** LCD SCREEN UPDATE RUTINE ****************************************
		 **********************************************************************************************************/
		if (lcd_update_evt) {
			//printf("Page: %d\r\n",cur_page);
			lcd_update();
			lcd_update_evt = 0U;
		}

		/***************************************************************************************************************
		 ******************************************* TIMER EVENT RUTINE ************************************************
		 ***************************************************************************************************************/
		if (timer_stop_evt) {
			//printf("Timer Stoped\r\n");
			HAL_TIM_Base_Stop_IT(&htim4);
			timer_stop_evt = 0;

		} else if ((timer_start_evt) && (dryer.state == START)) {
			//printf("Timer Started\r\n");
			HAL_TIM_Base_Start_IT(&htim4);
			timer_start_evt = 0;
		}

		/***************************************************************************************************************
		 ********************************************* KEYBOARD SCANNING ***********************************************
		 ********************************** INCREMENT DECREMENT OF TIMER AND MODE SET **********************************
		 ***************************************************************************************************************/
		// if (!door_open_flag && !coil_open_flag && !limit_timeout_flag) {
			if (!door_open_flag && !coil_open_flag && !limit_timeout_flag) {
//				printf("Scaning...\r\n");
				scaned_key = scan_keypad();

			if (dryer.state == START) {

				switch (scaned_key) {
					case NO_KEY_PRESSED:
						break;
					case LOW_KEY_PRESSED:
						lcd_update_evt = 1U;
						if (dryer.cycle == HEAT_CYCLE && dryer.heatTime > 60U)
						{
							dryer.heatTime -= 60U;
							printf("HeatTime = %d\r\n", dryer.heatTime);
						}
						else if (dryer.cycle == COOL_CYCLE && dryer.coolTime > 60U)
						{
							dryer.coolTime -= 60U;
							printf("CoolTime = %d\r\n", dryer.coolTime);
						}
						break;
					case MED_KEY_PRESSED:
						lcd_update_evt = 1U;
						dryer.state = INIT;
						dryer.mode = NO_MODE;
						cur_page = INIT_PAGE;
						timer_stop_evt = 1U;
						HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
						HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
						HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
						HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
						HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
						break;
					case HIGH_KEY_PRESSED:
						lcd_update_evt = 1U;
						if(dryer.cycle == HEAT_CYCLE){
							if(dryer.mode == LOW_LEVEL){
								if(dryer.heatTime < 1800U){
									dryer.heatTime += 60U;
								}
							}
							else if(dryer.mode == MED_LEVEL){
								if(dryer.heatTime < 2400U){
									dryer.heatTime += 60U;
								}
							}
							else if(dryer.mode == HIGH_LEVEL){
								if(dryer.heatTime < 3000U){
									dryer.heatTime += 60U;
								}
							}
							printf("HeatTime = %d\r\n", dryer.heatTime);
						} else if (dryer.cycle == COOL_CYCLE) {
								if (dryer.coolTime < 900U)
									dryer.coolTime += 60U;
								printf("CoolTime = %d\r\n", dryer.coolTime);
							}
						break;
					case FN_KEY_PRESSED:
						lcd_update_evt = 1U;
						dryer.heatTime = 0U;
						dryer.cycle = COOL_CYCLE;
						printf("HeatTime = %d\r\n", dryer.heatTime);
						printf("CoolTime = %d\r\n", dryer.coolTime);
						break;
				}
				scaned_key = NO_KEY_PRESSED;
			} else if (dryer.state == INIT) {

				switch (scaned_key) {
					case NO_KEY_PRESSED:
						break;
					
					case LOW_KEY_PRESSED:
						dryer.state = START;
						dryer.mode = LOW_LEVEL;
						dryer.heatTime = 600U;
						dryer.coolTime = 300U;
						dryer.beepTime = 30U;
						dryer.setTemp = 40U;
						dryer.cycle = HEAT_CYCLE;
						rot_left_cunt = 30U;
						rot_right_cunt = 0U;
						pasue_cunt = 0U;
						timer_start_evt = 1U;
						cur_page = LOW_LEVEL_PAGE;
						lcd_update_evt = 1U;
						op_update_evt = 1U;
						printf("LOW LEVEL\r\n");
						break;

					case MED_KEY_PRESSED:
						dryer.state = START;
						dryer.mode = MED_LEVEL;
						dryer.heatTime = 1200U;
						dryer.coolTime = 300U;
						dryer.beepTime = 30U;
						dryer.setTemp = 70U;
						dryer.cycle = HEAT_CYCLE;
						rot_left_cunt = 30U;
						rot_right_cunt = 0U;
						pasue_cunt = 0U;
						timer_start_evt = 1U;
						cur_page = MED_LEVEL_PAGE;
						lcd_update_evt = 1U;
						op_update_evt = 1U;
						printf("MED_LEVEL\r\n");
						break;

					case HIGH_KEY_PRESSED:
						dryer.state = START;
						dryer.mode = HIGH_LEVEL;
						dryer.heatTime = 1800U;
						dryer.coolTime = 300U;
						dryer.beepTime = 30U;
						dryer.setTemp = 90U;
						dryer.cycle = HEAT_CYCLE;
						rot_left_cunt = 30U;
						rot_right_cunt = 0U;
						pasue_cunt = 0U;
						timer_start_evt = 1U;
						cur_page = HIGH_LEVEL_PAGE;
						lcd_update_evt = 1U;
						op_update_evt = 1U;
						printf("HIGH_LEVEL\r\n");
						break;
				}
				scaned_key = NO_KEY_PRESSED;
			}else if(dryer.state == COMPLETE){
				if(scaned_key == MED_KEY_PRESSED){
					lcd_update_evt = 1U;
					dryer.state = INIT;
					dryer.mode = NO_MODE;
					cur_page = INIT_PAGE;
					timer_stop_evt = 1U;
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
					HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
					HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
					HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
					HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
				}
				scaned_key = NO_KEY_PRESSED;
			}
		}

		if (door_open_evt) {
			static volatile uint8_t temp_pin_state = 0U;

			temp_pin_state = HAL_GPIO_ReadPin(DOOR_SW_GPIO_Port, DOOR_SW_Pin);
			//printf("Pin: %d\r\n",temp_pin_state);

			if (temp_pin_state == HIGH) {

				if (dryer.state == COMPLETE) {
					timer_stop_evt = 1U;
					dryer.beepTime = 0U;
					dryer.state = INIT;
					dryer.mode = NO_MODE;
				}

				door_open_flag = 1U;
				timer_stop_evt = 1U;
				cur_page = DOOR_OPEN_PAGE;
				lcd_update_evt = 1U;
				dryer.state = STOP;

				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);

				
			} else {
				if (!limit_timeout_flag) {
					switch (dryer.mode) {
					case NO_MODE:
						cur_page = INIT_PAGE;
						dryer.state = INIT;
						break;
					case LOW_LEVEL:
						cur_page = LOW_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					case MED_LEVEL:
						cur_page = MED_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					case HIGH_LEVEL:
						cur_page = HIGH_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					}
				} else
					cur_page = LIMIT_SW_ERR_PAGE;
				lcd_update_evt = 1U;
				door_open_flag = 0U;
			}
			door_open_evt = 0U;
		}

		/*************************************************************************************************************
		 *************************************************HERTER COIL CHECK*******************************************
		 *************************************************************************************************************/
		if (!door_open_flag && !limit_timeout_flag && dryer.state != COMPLETE) {

			if (is_coil_open()) {

				coil_open_flag = 1U;
				timer_stop_evt = 1U;
				cur_page = HEATER_COIL_PAGE;
				lcd_update_evt = 1U;
				dryer.state = STOP;

				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);

			} else {
				coil_open_flag = 0U;
				if(dryer.state == STOP){
					switch (dryer.mode) {
					case NO_MODE:
						cur_page = INIT_PAGE;
						dryer.state = INIT;
						break;
					case LOW_LEVEL:
						cur_page = LOW_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					case MED_LEVEL:
						cur_page = MED_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					case HIGH_LEVEL:
						cur_page = HIGH_LEVEL_PAGE;
						dryer.state = START;
						timer_start_evt = 1U;
						break;
					}
					lcd_update_evt = 1U;
				}	
			}
		}

		/***************************************************************************************************************
		 ******************************************* LIMIT SWITCH ERROR RUTINE ************************************************
		 ***************************************************************************************************************/
		if (!door_open_flag && !coil_open_flag && !limit_timeout_flag) {
			if (dryer.state == START && limit_timeout == 0U) {
				limit_timeout_flag = 1U;
				lcd_update_evt = 1U;
				timer_stop_evt = 1U;

				dryer.state = STOP;
				dryer.mode = NO_MODE;
				cur_page = LIMIT_SW_ERR_PAGE;

				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
			}
		} else if (!door_open_flag && !coil_open_flag && limit_timeout_flag) {
			scaned_key = scan_keypad();
			if (scaned_key == MED_KEY_PRESSED) {
				dryer.state = INIT;
				cur_page = INIT_PAGE;
				limit_timeout = 30U;
				lcd_update_evt = 1U;
				limit_timeout_flag = 0U;
			}
		}

		/**********************************************************************************************************
		 *********************** CONTROL MACHINE IN START/STOP/COMPLETE PROCESS STATE *****************************
		 * @START STATE
		 * 		->CONTROLS FAN, DRUM MOTORS, HEATER SWTICH
		 * 		->READ TEMPRETURE AND MAINTAIN HEATER
		 * @COMPLETE STATE
		 * 		->TURNS OFF FAN, DRUM MOTORS, HEATER
		 * 		->TRUNS ON/OFF BEEP EVERY 3s FOR 30s
		 * 		->WATCHS DOOR OPEN
		 **********************************************************************************************************/
		if (!door_open_flag && !coil_open_flag && !limit_timeout_flag && op_update_evt) {

			if (dryer.state == START) {

				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, HIGH);

//				 if (otp) {
//				 	printf("HeaterTime = %d\r\n", dryer.heatTime);
//				 	printf("CoolTime = %d\r\n", dryer.coolTime);
//				 	printf("ROT_LEFT = %d\r\n", rot_left_cunt);
//				 	printf("PASUE = %d\r\n", pasue_cunt);
//				 	printf("ROT_RIGHT = %d\r\n", rot_right_cunt);
//				 	otp = 0;
//				 }

				//start Flip Flop
				if (pasue_cunt > 0) {
					HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
					HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				} else if (rot_left_cunt > 0) {
					HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, HIGH);
					HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				} else if (rot_right_cunt > 0) {
					HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
					HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, HIGH);
				}

				cur_temp = (int) Max6675_Read_Temp();//takes time
				//Maintain Heater
				if (dryer.cycle == HEAT_CYCLE && !limit_sw_open()) {
					if (cur_temp == -1 || cur_temp == 0) {
						HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
					} else if ((cur_temp > (dryer.setTemp + 2))
							|| (cur_temp <= 0)) {
						HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
					} else if ((cur_temp < (dryer.setTemp - 5))
							&& (cur_temp > 0)) {
						HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HIGH);
					}
				} else {
					HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
				}
			} else if (dryer.state == COMPLETE) {

				if (dryer.beepTime == 0) {
					HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
					cur_page = INIT_PAGE;
					lcd_update_evt = 1U;
					timer_stop_evt = 1U;
					dryer.state = INIT;
					dryer.mode = NO_MODE;
				}else{
					HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
				}
				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);

			} else {
				//beep off
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, LOW);
				HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_LEFT_GPIO_Port, DRUM_LEFT_Pin, LOW);
				HAL_GPIO_WritePin(DRUM_RIGHT_GPIO_Port, DRUM_RIGHT_Pin, LOW);
				HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, LOW);
			}
			op_update_evt = 0U;
		}
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//	static volatile uint8_t temp_pin_state = 0U;
//otp = 1;
	if (GPIO_Pin == DOOR_SW_Pin) {
		NVIC_DisableIRQ(EXTI0_IRQn);

		door_open_flag = 1U;
		door_open_evt = 1U;

		NVIC_EnableIRQ(EXTI0_IRQn);
//		printf("Door_EXTI\r\n");
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	
//	printf("Timer\r\n");

	if (dryer.heatTime > 0) {
		dryer.heatTime--;
	} else if (dryer.coolTime > 0) {
		dryer.coolTime--;
	} else if (dryer.state == COMPLETE) {
		dryer.beepTime--;
	}

	if ((dryer.heatTime == 0) && (dryer.cycle == HEAT_CYCLE)) {
		dryer.cycle = COOL_CYCLE;
	} else if ((dryer.coolTime == 0) && (dryer.cycle == COOL_CYCLE)) {
		//printf("Complete\r\n");
		dryer.state = COMPLETE;
		dryer.mode = NO_MODE;
		cur_page = COMPLETE_PAGE;
	}

	if (pasue_cunt > 0) {
		pasue_cunt--;
	} else if (rot_left_cunt > 0) {
		rot_left_cunt--;
		flipflop = 1;
	} else if (rot_right_cunt > 0) {
		rot_right_cunt--;
		flipflop = 0;
	} else {
		rot_sw_state = HAL_GPIO_ReadPin(SEL_ROT_SW_GPIO_Port, SEL_ROT_SW_Pin);
		if (rot_sw_state) {
			pasue_cunt = 0U;
			rot_left_cunt = 30U;
		} else {
			pasue_cunt = 3U;
			if (flipflop)rot_right_cunt = 30U;
			else rot_left_cunt = 30U;
		}
	}

	if (limit_sw_open() && (dryer.cycle == HEAT_CYCLE)) {
		if (limit_timeout > 0)
			limit_timeout--;
	} else {
		limit_timeout = 30U;
	    limit_timeout_flag = 0U;
	}
//	otp = 1;
	op_update_evt = 1U;
	lcd_update_evt = 1U;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
