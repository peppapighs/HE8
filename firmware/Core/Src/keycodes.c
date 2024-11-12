/*
 * keycodes.c
 *
 *  Created on: Nov 12, 2024
 *      Author: Pep
 */

#include "keycodes.h"

uint16_t keycode_to_system_control(uint16_t keycode) {
  switch (keycode) {
  case KC_PWR:
    return HID_USAGE_DESKTOP_SYSTEM_POWER_DOWN;
  case KC_WAKE:
    return HID_USAGE_DESKTOP_SYSTEM_WAKE_UP;
  case KC_SLEP:
    return HID_USAGE_DESKTOP_SYSTEM_SLEEP;

  default:
    return KC_NO;
  }
}

uint16_t keycode_to_consumer_control(uint16_t keycode) {
  switch (keycode) {
  case KC_BRIU:
    return HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT;
  case KC_BRID:
    return HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT;
  case KC_MPLY:
    return HID_USAGE_CONSUMER_PLAY_PAUSE;
  case KC_MNXT:
    return HID_USAGE_CONSUMER_SCAN_NEXT;
  case KC_MPRV:
    return HID_USAGE_CONSUMER_SCAN_PREVIOUS;
  case KC_MSTP:
    return HID_USAGE_CONSUMER_STOP;
  case KC_MUTE:
    return HID_USAGE_CONSUMER_MUTE;
  case KC_VOLU:
    return HID_USAGE_CONSUMER_VOLUME_INCREMENT;
  case KC_VOLD:
    return HID_USAGE_CONSUMER_VOLUME_DECREMENT;
  case KC_MSEL:
    return HID_USAGE_CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION;
  case KC_MAIL:
    return HID_USAGE_CONSUMER_AL_EMAIL_READER;
  case KC_CALC:
    return HID_USAGE_CONSUMER_AL_CALCULATOR;
  case KC_MYCM:
    return HID_USAGE_CONSUMER_AL_LOCAL_BROWSER;
  case KC_WSCH:
    return HID_USAGE_CONSUMER_AC_SEARCH;
  case KC_WHOM:
    return HID_USAGE_CONSUMER_AC_HOME;
  case KC_WBAK:
    return HID_USAGE_CONSUMER_AC_BACK;
  case KC_WFWD:
    return HID_USAGE_CONSUMER_AC_FORWARD;
  case KC_WSTP:
    return HID_USAGE_CONSUMER_AC_STOP;
  case KC_WREF:
    return HID_USAGE_CONSUMER_AC_REFRESH;
  case KC_WFAV:
    return HID_USAGE_CONSUMER_AC_BOOKMARKS;

  default:
    return KC_NO;
  }
}

uint8_t keycode_to_modifier(uint16_t keycode) {
  return 1 << (keycode - KC_LCTL);
}
