/*
 * firmware.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "firmware.h"

#include "tusb.h"

#include "main.h"

void firmware_init(void) {
  tusb_init(1, NULL);
  HAL_TIM_Base_Start_IT(&htim10);
}

void firmware_loop(void) { tud_task(); }
