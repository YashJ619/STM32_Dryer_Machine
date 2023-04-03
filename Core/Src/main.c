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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "machine.h"
#include "stdio.h"
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
uint8_t door_open_flag = 0U;
uint8_t timer_stop_evt = 0U;
uint8_t timer_start_evt = 0U;
uint8_t otp = 1; //not in use
uint8_t scaned_key = NO_KEY_PRESSED;

uint8_t rot_left_cunt = 30U;
uint8_t rot_right_cunt = 0U;
uint8_t pasue_cunt = 0U;

static uint8_t rot_sw_state = 0U;
static uint8_t flipflop = 0U;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 10);
	return ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	/* USER CODE BEGIN 2 */
	gpio_init();
	keypad_init();
	timer_init();

	lcd_gpio_init();
	lcd_init(0);
	lcd_begin(16, 4, 0);


	//HAL_UART_Transmit(&huart1, (uint8_t*)"HELLO\r\n", sizeof("HELLO\r\n"), 10);
	printf("Hello\r\n");
	dryer.state = INIT;
	dryer.mode = NO_MODE;

	lcd_set_cursor(1, 1);
	lcd_print("Dryer");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if(timer_stop_evt)
		{
			HAL_TIM_Base_Stop_IT(&htim4);
			timer_stop_evt = 0;

		}else if((timer_start_evt) && (dryer.state == START))
		{

			HAL_TIM_Base_Start_IT(&htim4);
			timer_start_evt = 0;

		}

		if (door_open_flag) {

			HAL_GPIO_WritePin(GPIOC, LED, LOW);

		} else {

			HAL_GPIO_WritePin(GPIOC, LED, HIGH);

			if (dryer.state == START) {

				scaned_key = scan_keypad();

				switch (scaned_key) {

				case LOW_KEY_PRESSED:
					switch (dryer.cycle) {

					case HEAT_CYCLE:
						if (dryer.heatTime > 60U)
							dryer.heatTime -= 60U;
						break;

					case COOL_CYCLE:
						if (dryer.coolTime > 60U)
							dryer.coolTime -= 60U;
						break;
					}
					break;

				case MED_KEY_PRESSED:
					dryer.state = INIT;
					timer_stop_evt = 1U;
					break;

				case HIGH_KEY_PRESSED:
					switch (dryer.cycle) {

					case HEAT_CYCLE:
						if ((dryer.heatTime < 1800U)
								&& (dryer.mode == LOW_LEVEL))
							dryer.heatTime += 60U;
						else if ((dryer.heatTime < 2400U)
								&& (dryer.mode == MED_LEVEL))
							dryer.heatTime += 60U;
						else if ((dryer.heatTime < 3000U)
								&& (dryer.mode == HIGH_LEVEL))
							dryer.heatTime += 60U;
						printf("HeatTime = %d\r\n", dryer.heatTime);
						break;

					case COOL_CYCLE:
						if (dryer.coolTime < 900U)
							dryer.coolTime += 60U;
						printf("CoolTime = %d\r\n", dryer.coolTime);
						break;
					}
					break;

				case FN_KEY_PRESSED:
					dryer.heatTime = 0U;
					dryer.cycle = COOL_CYCLE;
					printf("HeatTime = %d\r\n", dryer.heatTime);
					printf("CoolTime = %d\r\n", dryer.coolTime);
					break;
				}

				scaned_key = NO_KEY_PRESSED;

			} else if (dryer.state == INIT) {

				scaned_key = scan_keypad();

				switch (scaned_key) {

				case LOW_KEY_PRESSED:
					dryer.mode = LOW_LEVEL;
					dryer.state = START;
					dryer.heatTime = 600U;
					dryer.coolTime = 300U;
					dryer.beepTime = 30U;
					dryer.cycle = HEAT_CYCLE;
					rot_left_cunt = 30U;
					rot_right_cunt = 0U;
					pasue_cunt = 0U;
					timer_start_evt = 1U;
					HAL_UART_Transmit(&huart1, (uint8_t*) "LOW_LEVEL\r\n",
							sizeof("LOW_LEVEL\r\n"), 10);
					break;

				case MED_KEY_PRESSED:
					dryer.mode = MED_LEVEL;
					dryer.state = START;
					dryer.heatTime = 1200U;
					dryer.coolTime = 300U;
					dryer.beepTime = 30U;
					dryer.cycle = HEAT_CYCLE;
					rot_left_cunt = 30U;
					rot_right_cunt = 0U;
					pasue_cunt = 0U;
					timer_start_evt = 1U;
					HAL_UART_Transmit(&huart1, (uint8_t*) "MED_LEVEL\r\n",
							sizeof("MED_LEVEL\r\n"), 10);
					break;

				case HIGH_KEY_PRESSED:
					dryer.mode = HIGH_LEVEL;
					dryer.state = START;
					dryer.heatTime = 1800U;
					dryer.coolTime = 300U;
					dryer.beepTime = 30U;
					dryer.cycle = HEAT_CYCLE;
					rot_left_cunt = 30U;
					rot_right_cunt = 0U;
					pasue_cunt = 0U;
					timer_start_evt = 1U;
					HAL_UART_Transmit(&huart1, (uint8_t*) "HIGH_LEVEL\r\n",
							sizeof("HIGH_LEVEL\r\n"), 10);
					break;

				default:
					break;
				}

				scaned_key = NO_KEY_PRESSED;
			} //else if ends
		} //else ends

		if (door_open_flag) {

			HAL_GPIO_WritePin(FAN_PORT, FAN_PIN, LOW);
			HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, LOW);
			HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, LOW);
			HAL_GPIO_WritePin(OUTPUT_PORT, HEATER_PIN, LOW);

			if(dryer.state == COMPLETE)
			{
				//beep off
				timer_stop_evt = 1U;
				dryer.beepTime = 0U;
				dryer.state = INIT;
				dryer.mode = NO_MODE;
			}

		} else {

			if (dryer.state == START) {

				HAL_GPIO_WritePin(FAN_PORT, FAN_PIN, HIGH);

				if (otp) {
					printf("HeaterTime = %d\r\n", dryer.heatTime);
					printf("CoolTime = %d\r\n", dryer.coolTime);
					printf("ROT_LEFT = %d\r\n", rot_left_cunt);
					printf("PASUE = %d\r\n", pasue_cunt);
					printf("ROT_RIGHT = %d\r\n", rot_right_cunt);
					otp = 0;
				}

				//start Flip Flop
				if (pasue_cunt > 0) {

					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, LOW);
					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, LOW);

					if (pasue_cunt <= 1) {

						rot_sw_state = HAL_GPIO_ReadPin(INPUT_PORT,
						SEL_ROT_SW);

						if (rot_sw_state) {

							rot_left_cunt = 30U;
							rot_right_cunt = 0U;

						} else {

							if (flipflop) {

								rot_left_cunt = 0U;
								rot_right_cunt = 30U;

							} else {

								rot_left_cunt = 30U;
								rot_right_cunt = 0U;

							}
						}
					}
				} else if (rot_left_cunt > 0) {

					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, HIGH);
					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, LOW);
					flipflop = 1;

				} else if (rot_right_cunt > 0) {

					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, LOW);
					HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, HIGH);
					flipflop = 0;

				}

				if (dryer.cycle == HEAT_CYCLE) {

					HAL_GPIO_WritePin(OUTPUT_PORT, HEATER_PIN, HIGH);

				} else {

					HAL_GPIO_WritePin(OUTPUT_PORT, HEATER_PIN, LOW);

				}
			}else if(dryer.state == COMPLETE){

				if(dryer.beepTime == 0)
				{
					//beep off
					timer_stop_evt = 1U;
					dryer.state = INIT;
					dryer.mode = NO_MODE;
				}
				if(dryer.beepTime % 3 == 0)
				{
					//beep toggle
				}
				HAL_GPIO_WritePin(FAN_PORT, FAN_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, HEATER_PIN, LOW);
			}
			else{
				//beep off
				HAL_GPIO_WritePin(FAN_PORT, FAN_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_LEFT_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, DRUM_RIGHT_PIN, LOW);
				HAL_GPIO_WritePin(OUTPUT_PORT, HEATER_PIN, LOW);
			}
		}
	}
}
/* USER CODE END 3 */

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//otp = 1;
	if (GPIO_Pin == DOOR_SW) {

		uint8_t temp = HAL_GPIO_ReadPin(INPUT_PORT, DOOR_SW);

		if (temp == HIGH) {

			door_open_flag = 1;
			timer_stop_evt = 1;
			//stop

		} else {

			timer_start_evt = 1;
			door_open_flag = 0;

		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	HAL_UART_Transmit(&huart1, (uint8_t*) "TIMER\r\n", sizeof("TIMER\r\n"), 10);

	if (dryer.heatTime > 0) {

		dryer.heatTime--;

	} else if(dryer.coolTime > 0) {

		dryer.coolTime--;

	}else if(dryer.state == COMPLETE)
	{
		dryer.beepTime--;
	}

	if ((dryer.heatTime == 0) && (dryer.cycle == HEAT_CYCLE)) {

		dryer.cycle = COOL_CYCLE;

	}else if ((dryer.coolTime == 0) && (dryer.cycle == COOL_CYCLE)) {

		printf("Complete\r\n");
		dryer.state = COMPLETE;
		dryer.mode = NO_MODE;

	}

	if (pasue_cunt > 0) {

		pasue_cunt--;

	} else if (rot_left_cunt > 0) {

		rot_left_cunt--;

		if (rot_left_cunt == 0) {

			pasue_cunt = 3U;

		}

	} else if (rot_right_cunt > 0) {

		rot_right_cunt--;

		if (rot_right_cunt == 0) {

			pasue_cunt = 3U;

		}
	}
	otp = 1;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
