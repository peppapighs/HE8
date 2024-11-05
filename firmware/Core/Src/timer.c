/*
 * timer.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "tusb.h"

#include "main.h"
#include "usb_descriptors.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  static bool pressed = false;

  if (htim == &htim10) {
    if (!tud_hid_ready() ||
        HAL_GPIO_ReadPin(USER_GPIO_Port, USER_Pin) != GPIO_PIN_SET)
      return;

    if (!pressed) {
      uint8_t keycode[6] = {0};
      keycode[0] = HID_KEY_A;

      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
      pressed = true;
    } else {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
      pressed = false;
    }

    HAL_TIM_Base_Start_IT(&htim10);
  }
}
