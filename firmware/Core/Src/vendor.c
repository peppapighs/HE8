/*
 * vendor.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "tusb.h"

#include "firmware_config.h"
#include "keyboard.h"
#include "keyboard_config.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// Vendor Callbacks
//--------------------------------------------------------------------+

// Get the end of stack address from the linker script to load the boot flag
extern const uint32_t _estack;

const tusb_desc_webusb_url_t desc_url = {
    .bLength = 3 + sizeof(WEBUSB_URL) - 1,
    .bDescriptorType = TUSB_DESC_STRING, // WEBUSB URL type
    .bScheme = 1,                        // 0: http, 1: https
    .url = WEBUSB_URL,
};

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage
// (setup/data/ack) return false to stall control endpoint (e.g unsupported
// request)
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage,
                                tusb_control_request_t const *request) {
  switch (request->bmRequestType_bit.type) {
  case TUSB_REQ_TYPE_VENDOR:
    switch (request->bRequest) {
    case VENDOR_REQUEST_WEBUSB:
      if (stage == CONTROL_STAGE_SETUP) {
        // match vendor request in BOS descriptor
        // Get landing page url
        return tud_control_xfer(rhport, request, (void *)&desc_url,
                                desc_url.bLength);
      }
      return true;

    case VENDOR_REQUEST_MICROSOFT:
      if (stage == CONTROL_STAGE_SETUP && request->wIndex == 0x07) {
        // Get Microsoft OS 2.0 compatible descriptor
        uint16_t total_len;
        memcpy(&total_len, desc_ms_os_20 + 8, 2);

        return tud_control_xfer(rhport, request, (void *)desc_ms_os_20,
                                total_len);
      }
      return true;

    default:
      break;
    }
    break;

  case TUSB_REQ_TYPE_CLASS:
    switch (request->bRequest) {
    case VENDOR_CLASS_PROTOCOL_VERSION_CHECK:
      if (stage == CONTROL_STAGE_SETUP &&
          request->wValue == VENDOR_CLASS_PROTOCOL_VERSION) {
        // If the version matches, return success
        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_BOOTLOADER_JUMP:
      if (stage == CONTROL_STAGE_SETUP) {
        uint32_t *boot_flag = (uint32_t *)&_estack;

        // Set the boot flag to the magic value
        *boot_flag = BOOT_FLAG;
        NVIC_SystemReset();

        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_FACTORY_RESET:
      if (stage == CONTROL_STAGE_SETUP) {
        // TODO: Implement factory reset

        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_GET_KEYBOARD_CONFIG_LENGTH:
      if (stage == CONTROL_STAGE_SETUP) {
        uint16_t config_len = sizeof(keyboard_config_t);

        return tud_control_xfer(rhport, request, &config_len,
                                sizeof(config_len));
      }
      return true;

    case VENDOR_CLASS_GET_KEYBOARD_CONFIG:
      if (stage == CONTROL_STAGE_SETUP)
        return tud_control_xfer(rhport, request, &keyboard_config,
                                sizeof(keyboard_config));
      return true;

    default:
      break;
    }
    break;

  default:
    break;
  }

  // stall unknown request
  return false;
}
