#include "led_control.h"

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;

// each data unit is a bit, 3 bytes per led is 24
static uint8_t s_timer_led_data[LED_COUNT * 24] = {0};


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


static void convert_led_to_timings(uint8_t * timing, const LED_t * leds, size_t led_count)
{
	for (size_t i = 0; i < led_count; i++)
	{
		convert_byte_to_timings(timing + 24 * i, leds[i].green);
		convert_byte_to_timings(timing + 24 * i + 8, leds[i].blue);
		convert_byte_to_timings(timing + 24 * i + 16, leds[i].red);
	}
	
}


int write_leds(const LED_t * leds, size_t led_count)
{
	if (TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_1) == HAL_BUSY)
	{
		return -1;
	}
	convert_led_to_timings(s_timer_led_data, leds, led_count);
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)s_timer_led_data, sizeof(s_timer_led_data));
}