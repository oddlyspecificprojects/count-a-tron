#include "led_control.h"

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;

// each data unit is a bit, 3 bytes per led is 24
static uint8_t s_timer_led_data[LED_COUNT * 24 + 50] = {0};

/**
 * @brief Convert a byte to WS2812 timings
 * 
 * @param timing timing buffer
 * @param data byte to convert
 */
static void convert_byte_to_timings(uint8_t * timing, uint8_t data)
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

/**
 * @brief Convert a single instance of led to timings
 * 
 * @param timing timing buffer
 * @param led led to convert
 */
static void convert_led_to_timings(uint8_t * timing, const LED_t led)
{
	convert_byte_to_timings(timing, led.green);
	convert_byte_to_timings(timing + 8, led.red);
	convert_byte_to_timings(timing + 16, led.blue);
	
}


int write_leds(const LED_t * leds, size_t led_count)
{
	if (TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY)
	{
		return -1;
	}
	for (size_t i = 0; i < led_count; i++)
	{
		convert_led_to_timings(s_timer_led_data + 24 * i, leds[i]);
	}
	
	
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)s_timer_led_data, sizeof(s_timer_led_data));
	return 0;
}