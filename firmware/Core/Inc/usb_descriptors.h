/*
 * usb_descriptors.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_USB_DESCRIPTORS_H_
#define INC_USB_DESCRIPTORS_H_

#include <stdint.h>

//--------------------------------------------------------------------+
// Vendor Requests
//--------------------------------------------------------------------+

enum {
  VENDOR_REQUEST_WEBUSB,
  VENDOR_REQUEST_MICROSOFT,
};

//--------------------------------------------------------------------+
// Vendor Class Requests
//--------------------------------------------------------------------+

#define VENDOR_CLASS_PROTOCOL_VERSION 0x0100

enum {
  VENDOR_CLASS_PROTOCOL_VERSION_CHECK = 0x01,
  VENDOR_CLASS_BOOTLOADER_JUMP = 0x02,
  VENDOR_CLASS_FACTORY_RESET = 0x03,
  VENDOR_CLASS_GET_KEYBOARD_CONFIG_LENGTH = 0x04,
  VENDOR_CLASS_GET_KEYBOARD_CONFIG = 0x05,
  VENDOR_CLASS_GET_KEY_SWITCH_CONFIG = 0x06,
  VENDOR_CLASS_SET_KEY_SWITCH_CONFIG = 0x07,
  VENDOR_CLASS_RESET_KEY_SWITCH_CONFIG = 0x08,
  VENDOR_CLASS_GET_KEYMAP = 0x09,
  VENDOR_CLASS_SET_KEYMAP = 0x0A,
  VENDOR_CLASS_RESET_KEYMAP = 0x0B,
  VENDOR_CLASS_GET_SWITCH_PROFILE = 0x0C,
  VENDOR_CLASS_SET_SWITCH_PROFILE = 0x0D,
  VENDOR_CLASS_GET_KEY_SWITCH_STATE = 0x0E,
};

//--------------------------------------------------------------------+
// Report IDs
//--------------------------------------------------------------------+

enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_SYSTEM_CONTROL,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_MOUSE,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT,
};

extern uint8_t const desc_ms_os_20[];

#endif /* INC_USB_DESCRIPTORS_H_ */
