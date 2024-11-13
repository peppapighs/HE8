/*
 * vendor.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "tusb.h"

#include "firmware_config.h"
#include "usb_descriptors.h"

const tusb_desc_webusb_url_t desc_url = {
    .bLength = 3 + sizeof(WEBUSB_URL) - 1,
    .bDescriptorType = TUSB_DESC_STRING, // WEBUSB URL type
    .bScheme = 1,                        // 0: http, 1: https
    .url = WEBUSB_URL};

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage
// (setup/data/ack) return false to stall control endpoint (e.g unsupported
// request)
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage,
                                tusb_control_request_t const *request) {
  // nothing to with DATA & ACK stage
  if (stage != CONTROL_STAGE_SETUP)
    return true;

  switch (request->bmRequestType_bit.type) {
  case TUSB_REQ_TYPE_VENDOR:
    switch (request->bRequest) {
    case VENDOR_REQUEST_WEBUSB:
      // match vendor request in BOS descriptor
      // Get landing page url
      return tud_control_xfer(rhport, request, (void *)&desc_url,
                              desc_url.bLength);

    case VENDOR_REQUEST_MICROSOFT:
      if (request->wIndex == 0x07) {
        // Get Microsoft OS 2.0 compatible descriptor
        uint16_t total_len;
        memcpy(&total_len, desc_ms_os_20 + 8, 2);

        return tud_control_xfer(rhport, request, (void *)desc_ms_os_20,
                                total_len);
      }
      break;

    default:
      break;
    }
    break;

  case TUSB_REQ_TYPE_CLASS:
    if (request->bRequest == 0x22) {
      // response with status OK
      return tud_control_status(rhport, request);
    }
    break;

  default:
    break;
  }

  // stall unknown request
  return false;
}
