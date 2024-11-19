/*
 * usb_descriptors.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_USB_DESCRIPTORS_H_
#define INC_USB_DESCRIPTORS_H_

#include <stdint.h>

#include "class/hid/hid.h"
#include "tusb.h"

#include "firmware_config.h"

//--------------------------------------------------------------------+
// Vendor Requests
//--------------------------------------------------------------------+

enum {
  VENDOR_REQUEST_WEBUSB,
  VENDOR_REQUEST_MICROSOFT,
};

extern uint8_t const desc_ms_os_20[];

//--------------------------------------------------------------------+
// Report IDs
//--------------------------------------------------------------------+

enum {
  REPORT_ID_KEYBOARD = 1,
  // `REPORT_ID_KB_NKRO` must be next to `REPORT_ID_KEYBOARD` so that we can
  // iterate through the rest of the reports
  REPORT_ID_KB_NKRO,
  REPORT_ID_SYSTEM_CONTROL,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_MOUSE,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT,
};

//--------------------------------------------------------------------+
// Additional HID Report Descriptors
//--------------------------------------------------------------------+

#define HID_REPORT_DESC_KB_NKRO(...)                                           \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                      \
      HID_USAGE(HID_USAGE_DESKTOP_KEYBOARD),                                   \
      HID_COLLECTION(HID_COLLECTION_APPLICATION),                              \
                                                                               \
      /* Report ID if any */                                                   \
      __VA_ARGS__                                                              \
                                                                               \
      /* 8 bits Modifier Keys (Shift, Control, Alt) */                         \
      HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD),                                 \
      HID_USAGE_MIN(224), HID_USAGE_MAX(231), HID_LOGICAL_MIN(0),              \
      HID_LOGICAL_MAX(1), HID_REPORT_COUNT(8), HID_REPORT_SIZE(1),             \
      HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                       \
                                                                               \
      /* 8 bit reserved */                                                     \
      HID_REPORT_COUNT(1), HID_REPORT_SIZE(8), HID_INPUT(HID_CONSTANT),        \
                                                                               \
      /* Output 5-bit LED Indicator Kana | Compose | ScrollLock | CapsLock |   \
          NumLock */                                                           \
      HID_USAGE_PAGE(HID_USAGE_PAGE_LED), HID_USAGE_MIN(1), HID_USAGE_MAX(5),  \
      HID_REPORT_COUNT(5), HID_REPORT_SIZE(1),                                 \
      HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                      \
                                                                               \
      /* led padding */                                                        \
      HID_REPORT_COUNT(1), HID_REPORT_SIZE(3), HID_OUTPUT(HID_CONSTANT),       \
                                                                               \
      /* 6-byte padding for compatibility with 6-KRO HID report */             \
      HID_REPORT_COUNT(48), HID_REPORT_SIZE(1), HID_INPUT(HID_CONSTANT),       \
                                                                               \
      /* NKRO Bitmap */                                                        \
      HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD), HID_USAGE_MIN(0),               \
      HID_USAGE_MAX(NUM_NKRO_BYTES * 8 - 1), HID_LOGICAL_MIN(0),               \
      HID_LOGICAL_MAX(1), HID_REPORT_COUNT(NUM_NKRO_BYTES * 8),                \
      HID_REPORT_SIZE(1), HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),   \
      HID_COLLECTION_END

typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keycodes[6];
  uint8_t bitmap[NUM_NKRO_BYTES];
} __attribute__((packed)) hid_kb_nkro_report_t;

_Static_assert(1 + sizeof(hid_kb_nkro_report_t) <= CFG_TUD_HID_EP_BUFSIZE,
               "NKRO report size exceeds HID buffer size");

#endif /* INC_USB_DESCRIPTORS_H_ */
