/*
 * vendor.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "vendor.h"

#include "firmware_config.h"
#include "key_switch.h"
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

static uint8_t vendor_buffer[VENDOR_BUFFER_SIZE];

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
      if (stage == CONTROL_STAGE_SETUP) {
        if (request->wValue != VENDOR_CLASS_PROTOCOL_VERSION)
          // Wrong protocol version
          return false;

        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_PROTOCOL_FIRMWARE_VERSION:
      if (stage == CONTROL_STAGE_SETUP) {
        if (request->wLength != sizeof(uint16_t))
          // Wrong request length
          return false;

        uint16_t firmware_version = FIRMWARE_VERSION;

        return tud_control_xfer(rhport, request, &firmware_version,
                                sizeof(firmware_version));
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
        memcpy(&keyboard_config, &default_keyboard_config,
               sizeof(keyboard_config));

        save_keyboard_config();

        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_GET_KEYBOARD_CONFIG:
      if (stage == CONTROL_STAGE_SETUP) {
        if (request->wLength != sizeof(keyboard_config))
          // Wrong request length
          return false;

        return tud_control_xfer(rhport, request, &keyboard_config,
                                sizeof(keyboard_config));
      }
      return true;

    case VENDOR_CLASS_SET_KEY_SWITCH_CONFIG:
      if (stage == CONTROL_STAGE_SETUP) {
        return tud_control_xfer(rhport, request, vendor_buffer,
                                request->wLength);
      } else if (stage == CONTROL_STAGE_DATA) {
        uint16_t const payload_size =
            sizeof(uint8_t)               // Profile index
            + sizeof(uint8_t)             // Key index
            + sizeof(key_switch_config_t) // Key switch configuration
            ;

        for (uint16_t i = 0; i < request->wLength; i += payload_size) {
          uint8_t profile_index = vendor_buffer[i];
          uint8_t key_index = vendor_buffer[i + 1];
          key_switch_config_t *config =
              (key_switch_config_t *)&vendor_buffer[i + 2];

          if (profile_index >= NUM_PROFILES || key_index >= NUM_KEYS)
            // Invalid profile or key index
            return false;

          memcpy(&keyboard_config.key_switch_config[profile_index][key_index],
                 config, sizeof(key_switch_config_t));

          save_key_switch_config(profile_index, key_index);
        }

        return true;
      }
      return true;

    case VENDOR_CLASS_RESET_KEY_SWITCH_CONFIG:
      if (stage == CONTROL_STAGE_SETUP) {
        return tud_control_xfer(rhport, request, vendor_buffer,
                                request->wLength);
      } else if (stage == CONTROL_STAGE_DATA) {
        uint16_t const payload_size = sizeof(uint8_t)   // Profile index
                                      + sizeof(uint8_t) // Key index
            ;

        for (uint16_t i = 0; i < request->wLength; i += payload_size) {
          uint8_t profile_index = vendor_buffer[i];
          uint8_t key_index = vendor_buffer[i + 1];

          if (profile_index >= NUM_PROFILES || key_index >= NUM_KEYS)
            // Invalid profile or key index
            return false;

          memcpy(&keyboard_config.key_switch_config[profile_index][key_index],
                 &default_keyboard_config
                      .key_switch_config[profile_index][key_index],
                 sizeof(key_switch_config_t));

          save_key_switch_config(profile_index, key_index);
        }

        return true;
      }
      return true;

    case VENDOR_CLASS_SET_KEYMAP:
      if (stage == CONTROL_STAGE_SETUP) {
        return tud_control_xfer(rhport, request, vendor_buffer,
                                request->wLength);
      } else if (stage == CONTROL_STAGE_DATA) {
        uint16_t const payload_size = sizeof(uint8_t)    // Profile index
                                      + sizeof(uint8_t)  // Layer index
                                      + sizeof(uint8_t)  // Key index
                                      + sizeof(uint16_t) // Keycode
            ;

        for (uint16_t i = 0; i < request->wLength; i += payload_size) {
          uint8_t profile_index = vendor_buffer[i];
          uint8_t layer_index = vendor_buffer[i + 1];
          uint8_t key_index = vendor_buffer[i + 2];
          uint16_t *keycode = (uint16_t *)&vendor_buffer[i + 3];

          if (profile_index >= NUM_PROFILES || layer_index >= NUM_LAYERS ||
              key_index >= NUM_KEYS)
            // Invalid profile, layer, or key index
            return false;

          keyboard_config.keymap[profile_index][layer_index][key_index] =
              *keycode;

          save_keymap(profile_index, layer_index, key_index);
        }

        return true;
      }
      return true;

    case VENDOR_CLASS_RESET_KEYMAP:
      if (stage == CONTROL_STAGE_SETUP) {
        return tud_control_xfer(rhport, request, vendor_buffer,
                                request->wLength);
      } else if (stage == CONTROL_STAGE_DATA) {
        uint16_t const payload_size = sizeof(uint8_t)   // Profile index
                                      + sizeof(uint8_t) // Layer index
                                      + sizeof(uint8_t) // Key index
            ;

        for (uint16_t i = 0; i < request->wLength; i += payload_size) {
          uint8_t profile_index = vendor_buffer[i];
          uint8_t layer_index = vendor_buffer[i + 1];
          uint8_t key_index = vendor_buffer[i + 2];

          if (profile_index >= NUM_PROFILES || layer_index >= NUM_LAYERS ||
              key_index >= NUM_KEYS)
            // Invalid profile, layer, or key index
            return false;

          keyboard_config.keymap[profile_index][layer_index][key_index] =
              default_keyboard_config
                  .keymap[profile_index][layer_index][key_index];

          save_keymap(profile_index, layer_index, key_index);
        }

        return true;
      }
      return true;

    case VENDOR_CLASS_SET_SWITCH_PROFILE:
      if (stage == CONTROL_STAGE_SETUP) {
        if (request->wValue >= SWITCH_PROF_COUNT)
          // Invalid switch profile
          return false;

        set_switch_profile(request->wValue);

        return tud_control_status(rhport, request);
      }
      return true;

    case VENDOR_CLASS_GET_KEY_SWITCH_STATE:
      if (stage == CONTROL_STAGE_SETUP) {
        if (request->wLength != sizeof(key_switches))
          // Wrong request length
          return false;

        return tud_control_xfer(rhport, request, key_switches,
                                sizeof(key_switches));
      }
      return true;

    case VENDOR_CLASS_RECALIBRATE:
      if (stage == CONTROL_STAGE_SETUP) {
        start_calibrating_key_switches();

        return tud_control_xfer(rhport, request, vendor_buffer,
                                request->wLength);
      }
      return true;

    case VENDOR_CLASS_SET_NKRO:
      if (stage == CONTROL_STAGE_SETUP) {
        set_nkro(request->wValue & 1);

        return tud_control_status(rhport, request);
      }
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
