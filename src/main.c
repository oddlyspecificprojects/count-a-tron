/* USER CODE BEGIN Header */
/**
	******************************************************************************
	* @file           : main.c
	* @brief          : Main program body
	******************************************************************************
	* @attention
	*
	* Copyright (c) 2022 STMicroelectronics.
	* All rights reserved.
	*
	* This software is licensed under terms that can be found in the LICENSE file
	* in the root directory of this software component.
	* If no LICENSE file comes with this software, it is provided AS-IS.
	*
	******************************************************************************
	*/
/* USER CODE END Header */
#include <string.h>
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init.h"
#include "led_output_generator.h"
#include "led_control.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t alarm_tick_count = 0;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;
/* USER CODE BEGIN PV */
LED_t led_data[LED_COUNT] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
	* @brief  The application entry point.
	* @retval int
	*/
int main(void)
{
	/* USER CODE BEGIN 1 */
	uint16_t counter = 0;
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
	MX_DMA_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
	
	HAL_GPIO_WritePin(FIZZ_GPIO_Port, FIZZ_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		counter = __HAL_TIM_GET_COUNTER(&htim1);
		if (counter > 0)
		{
			HAL_GPIO_WritePin(FIZZ_GPIO_Port, FIZZ_Pin, counter % 3 == 0);
			HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, counter % 5 == 0);
		}
		else
		{
			HAL_GPIO_WritePin(FIZZ_GPIO_Port, FIZZ_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);			
		}
		if (alarm_tick_count != 0)
		{
			counter_to_leds(led_data, LED_COUNT, alarm_tick_count);
		}
		else
		{
			counter_to_leds(led_data, LED_COUNT, counter > 1024 ? 1024 : counter);
		}
		write_leds(led_data, LED_COUNT);
		HAL_Delay(2);		
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}



/* USER CODE BEGIN 4 */

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
	while (1)
	{
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
