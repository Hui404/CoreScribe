/*
 * MyDelay.c
 *
 *  Created on: Feb 21, 2026
 *      Author: xuuuu404
 */

#include "MyDelay.h"
#include "FreeRTOS.h"
#include "task.h"

static uint32_t cycles_per_us;

void MyDelay_Init(void)
{
	cycles_per_us = SystemCoreClock / 1000000U;

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

}


void MyDelay_us(unsigned long nus)
{
	uint32_t start;
	uint32_t delay_cycles;

	if(cycles_per_us == 0U) {
		cycles_per_us = SystemCoreClock / 1000000U;
	}

	start = DWT->CYCCNT;
	delay_cycles = (uint32_t)nus * cycles_per_us;

	while((DWT->CYCCNT - start) < delay_cycles) {
		__NOP();
	}

}


void MyDelay_ms(unsigned int nms)
{
	if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
		vTaskDelay(pdMS_TO_TICKS(nms > 0U ? nms : 1U));
		return;
	}

	HAL_Delay(nms);
}

