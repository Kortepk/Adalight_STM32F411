/*
 * WS2812.c
 *
 *  Created on: Jul 6, 2024
 *      Author: Kortep
 */

#include "main.h"

void ConvertColorToTim(struct RGB_LED* LED_data, uint8_t* PwmValue)
{
	uint32_t i, j, PwmIndex = 0, col = 0;
	for(i = 0; i < LED_NUM; i ++)
	{
		col = (LED_data[i].g << 23) | (LED_data[i].r << 16) | (LED_data[i].b);
		for(j = 23; j >= 0; j --)
		{
			if(col & (1 << j))  // We check every bit
				PwmValue[PwmIndex] = 2; // Filling PWM 2/3
			else
				PwmValue[PwmIndex] = 1; // Filling PWM 1/3
			PwmIndex ++;
		}
	}

	for(i = 0; i < 50; i ++, PwmIndex ++) // Create 62.5 us delay
		PwmValue[PwmIndex] = 0;
}
