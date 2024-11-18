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
