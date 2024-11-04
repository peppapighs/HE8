/*
 * usb_descriptors.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_USB_DESCRIPTORS_H_
#define INC_USB_DESCRIPTORS_H_

#include <stdint.h>

enum {
  VENDOR_REQUEST_WEBUSB,
  VENDOR_REQUEST_MICROSOFT,
};

extern uint8_t const desc_ms_os_20[];

#endif /* INC_USB_DESCRIPTORS_H_ */
