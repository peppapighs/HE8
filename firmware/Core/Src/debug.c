/*
 * debug.c
 *
 *  Created on: Nov 14, 2024
 *      Author: Pep
 */

#include "debug.h"

#ifdef DEBUG

#include "stm32f4xx_hal.h"

//--------------------------------------------------------------------+
// Debug Information
//--------------------------------------------------------------------+

debug_info_t debug_info;

//--------------------------------------------------------------------+
// Debug Functions
//--------------------------------------------------------------------+

void debug_init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void debug_counter_start(void) { DWT->CYCCNT = 0; }

uint32_t debug_counter_stop(void) { return DWT->CYCCNT; }

#endif
