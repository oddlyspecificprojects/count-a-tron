#include <string.h>

#include "led_output_generator.h"

extern uint8_t brightness;

const LED_t colors[] = {
	{255, 0, 0},
	{0, 0, 255},
	{255, 0, 255}
};

static void apply_color(LED_t * led, const LED_t color)
{
	led->blue = color.blue;
	led->red = color.red;
}

static uint8_t inter_second_index(uint32_t counter)
{
	return counter % 16;
}

void counter_to_leds(LED_t * leds, uint32_t led_count, uint32_t counter)
{
	if (counter > 1024)
	{
		counter = 1024;
	}
	uint32_t seconds = counter / 16;
	uint32_t last_seconds_led = seconds / 4;
	memset(leds, 0, led_count * sizeof(LED_t));
	leds[inter_second_index(counter)].green = 255;
	for (size_t i = 0; i < last_seconds_led; i++)
	{
		leds[i].red = 255;
	}
	leds[last_seconds_led].red = 64 * (seconds % 4);
}