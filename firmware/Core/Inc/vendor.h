/*
 * vendor.h
 *
 *  Created on: Nov 18, 2024
 *      Author: Pep
 */

#ifndef INC_VENDOR_H_
#define INC_VENDOR_H_

#include "keyboard.h"

//--------------------------------------------------------------------+
// Vendor Class Requests
//--------------------------------------------------------------------+

#define VENDOR_CLASS_PROTOCOL_VERSION 0x0100

enum {
  VENDOR_CLASS_PROTOCOL_VERSION_CHECK = 0x01,
  VENDOR_CLASS_PROTOCOL_FIRMWARE_VERSION = 0x02,
  VENDOR_CLASS_BOOTLOADER_JUMP = 0x03,
  VENDOR_CLASS_FACTORY_RESET = 0x04,
  VENDOR_CLASS_GET_KEYBOARD_CONFIG = 0x05,
  VENDOR_CLASS_SET_KEY_SWITCH_CONFIG = 0x06,
  VENDOR_CLASS_RESET_KEY_SWITCH_CONFIG = 0x07,
  VENDOR_CLASS_SET_KEYMAP = 0x08,
  VENDOR_CLASS_RESET_KEYMAP = 0x09,
  VENDOR_CLASS_SET_SWITCH_PROFILE = 0x0A,
  VENDOR_CLASS_GET_KEY_SWITCH_STATE = 0x0B,
};

#endif /* INC_VENDOR_H_ */
