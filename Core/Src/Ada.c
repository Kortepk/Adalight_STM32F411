/*
 * Ada.c
 *
 *  Created on: Jul 6, 2024
 *      Author: Kortep
 */

#include <stm32f4xx_hal.h>
#include "main.h"

struct RGB_LED LED_data[LED_NUM];

/*
 * Function convert raw rgb value to array rgb
 * arg: StartIndex - pointer to an rx buffer, len - length rx buffer
 * return: The length processed by the function
 */
uint32_t Processing_rx_buffer(uint8_t* StartIndex, uint32_t len)
{
	uint32_t i = 0, Shift;
	for(i = 0; i < len - 2; i++)
	{
		if(		StartIndex[i] == 'A' &&
				StartIndex[i+1] == 'd' &&
				StartIndex[i+2] == 'a'
		) // We are looking for a pattern
			break;
	}

	if(i >= (len - 2)) // Couldn't find "Ada"
		return len - 2; // Say necessary erase all buffer

	if(i + EXP_ADA_SIZE > len)
		return i;   // Say necessary trim a row up to a certain index and then accept more data

	uint8_t* RunningIndex = StartIndex; // Move index to size_data
	RunningIndex += i + 3;
	uint8_t check_sum = RunningIndex[0] ^ RunningIndex[1] ^ 0x55;

	Shift = i + 5;

	if(check_sum != RunningIndex[2])
		return Shift; // Say not valid control sum

	uint16_t size_data = ((uint16_t)RunningIndex[0]) << 8 | ((uint16_t)RunningIndex[1]) + (uint16_t)1;

	RunningIndex += 3;

	for(i = 0; i < size_data; i++, RunningIndex += 3)
	{
		LED_data[i].r = RunningIndex[0];
		LED_data[i].g = RunningIndex[1];
		LED_data[i].b = RunningIndex[2];
	}

	return Shift + i * 3;
}


