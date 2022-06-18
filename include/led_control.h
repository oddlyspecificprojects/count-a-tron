#pragma once
#include "main.h"

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t res;
} LED_t __attribute__ ((aligned (4)));

/**
 * @brief This function displays the colors on the LED
 * @note: not blocking! check return value to see if busy
 * 
 * @param leds led colors
 * @param led_count how many leds
 * @return 0 write succesfull
 * @return -1 busy writing
 */
int write_leds(const LED_t * leds, size_t led_count);