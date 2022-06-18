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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t res;
} LED_t __attribute__ ((aligned (4)));

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t brightness = 255;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch1;
/* USER CODE BEGIN PV */
LED_t led_data[LED_COUNT] = {0};
uint8_t timer_data_buffer[LED_COUNT * 3 * 8] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void convert_byte_to_timings(uint8_t * timing, uint8_t data);
void convert_led_to_timings(uint8_t * timing, const LED_t * leds, size_t led_count);
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
	MX_RTC_Init();
	MX_DMA_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
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
		for (size_t i = 0; i < LED_COUNT; i++)
		{
			led_data[i].green = (counter + i) != 0 && (counter + i) % 11 == 0 ? brightness : 0;
			led_data[i].red = (counter + i) != 0 && (counter + i) % 17 == 0 ? brightness : 0;
			led_data[i].blue = (counter + i) != 0 && (counter + i) % 23 == 0 ? brightness : 0;
		}
		convert_led_to_timings(timer_data_buffer, led_data, LED_COUNT);
		HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)timer_data_buffer, sizeof(timer_data_buffer));
		HAL_Delay(10);		
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

void convert_led_to_timings(uint8_t * timing, const LED_t * leds, size_t led_count)
{
	for (size_t i = 0; i < led_count; i++)
	{
		convert_byte_to_timings(timing + 24 * i, leds[i].green);
		convert_byte_to_timings(timing + 24 * i + 8, leds[i].blue);
		convert_byte_to_timings(timing + 24 * i + 16, leds[i].red);
	}
	
}

void convert_byte_to_timings(uint8_t * timing, uint8_t data)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (data & (1<<(7-i)))
		{
			timing[i] = 60;
		}
		else
		{
			timing[i] = 30;
		}
	}
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
