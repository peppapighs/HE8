/*
 * usb_descriptors.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "usb_descriptors.h"

#include "bsp/board_api.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+

tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0210, // Must be at least USB 2.1 to support WebUSB

    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,

    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = USB_VENDOR_ID,
    .idProduct = USB_PRODUCT_ID,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const *tud_descriptor_device_cb(void) {
  return (uint8_t const *)&desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_SYSTEM_CONTROL(HID_REPORT_ID(REPORT_ID_SYSTEM_CONTROL)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE)),
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(REPORT_ID_GAMEPAD)),
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
  return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum { ITF_NUM_HID, ITF_NUM_VENDOR, ITF_NUM_TOTAL };

#define CONFIG_TOTAL_LEN                                                       \
  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_VENDOR_DESC_LEN)

#define EPNUM_HID 0x81
#define EPNUM_VENDOR_IN 0x82
#define EPNUM_VENDOR_OUT 0x02

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 500),

    // Interface number, string index, protocol, report descriptor len, EP In
    // address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE,
                       sizeof(desc_hid_report), EPNUM_HID,
                       CFG_TUD_HID_EP_BUFSIZE, 1),

    // Interface number, string index, EP Out & IN address, EP size
    TUD_VENDOR_DESCRIPTOR(ITF_NUM_VENDOR, 4, EPNUM_VENDOR_OUT,
                          0x80 | EPNUM_VENDOR_IN,
                          TUD_OPT_HIGH_SPEED ? 512 : 64),
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
  return desc_configuration;
}

//--------------------------------------------------------------------+
// BOS Descriptor
//--------------------------------------------------------------------+

#define BOS_TOTAL_LEN                                                          \
  (TUD_BOS_DESC_LEN + TUD_BOS_WEBUSB_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

#define MS_OS_20_SET_HEADER_LEN 0x0A
#define MS_OS_20_SUBSET_HEADER_CONFIGURATION_LEN 0x08
#define MS_OS_20_SUBSET_HEADER_FUNCTION_LEN 0x08
#define MS_OS_20_FEATURE_COMPATBLE_ID_LEN 0x14
#define MS_OS_20_FEATURE_REG_PROPERTY_LEN 0x84
#define MS_OS_20_SUBSET_FUNCTION_LEN                                           \
  (MS_OS_20_SUBSET_HEADER_FUNCTION_LEN + MS_OS_20_FEATURE_COMPATBLE_ID_LEN +   \
   MS_OS_20_FEATURE_REG_PROPERTY_LEN)

#define MS_OS_20_SUBSET_CONFIGURATION_LEN                                      \
  (MS_OS_20_SUBSET_HEADER_CONFIGURATION_LEN +                                  \
   ITF_NUM_TOTAL * MS_OS_20_SUBSET_FUNCTION_LEN)
#define MS_OS_20_DESC_LEN                                                      \
  (MS_OS_20_SET_HEADER_LEN + MS_OS_20_SUBSET_CONFIGURATION_LEN)

// BOS Descriptor is required for webUSB
uint8_t const desc_bos[] = {
    // total length, number of device caps
    TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 2),

    // Vendor Code, iLandingPage
    TUD_BOS_WEBUSB_DESCRIPTOR(VENDOR_REQUEST_WEBUSB, 1),

    // Microsoft OS 2.0 descriptor
    TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)};

uint8_t const *tud_descriptor_bos_cb(void) { return desc_bos; }

// Microsoft OS 2.0 Descriptors
// https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-os-2-0-descriptors-specification
uint8_t const desc_ms_os_20[] = {
    // Set header length
    U16_TO_U8S_LE(MS_OS_20_SET_HEADER_LEN),
    // Set header descriptor type
    U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR),
    // Minimum supported Windows version (Windows 8.1)
    U32_TO_U8S_LE(0x06030000),
    // Total length of MS OS 2.0 descriptor
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

    // Subset header length
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION_LEN),
    // Subset header descriptor type
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION),
    // Configuration index
    0x00,
    // Reserved
    0x00,
    // Length of this configuration subset including this header
    U16_TO_U8S_LE(MS_OS_20_SUBSET_CONFIGURATION_LEN),

    // Function subset header length
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION_LEN),
    // Function subset header descriptor type
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION),
    // First interface
    ITF_NUM_HID,
    // Reserved
    0x00,
    // Length of this function subset including this header
    U16_TO_U8S_LE(MS_OS_20_SUBSET_FUNCTION_LEN),

    // Feature descriptor length
    U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID_LEN),
    // Feature descriptor type
    U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID),
    // Compatible ID
    // HID devices do not require driver installation
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Sub-compatible ID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // Registry property descriptor length
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY_LEN),
    // Registry property descriptor type
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
    // Property data type (REG_MULTI_SZ)
    U16_TO_U8S_LE(0x0007),
    // Property name length
    U16_TO_U8S_LE(0x002A),
    // Property name
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00,
    'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00,
    'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00,
    0x00,
    // Property data length
    U16_TO_U8S_LE(0x0050),
    // Property data (randomly generated GUID)
    '{', 0x00, 'E', 0x00, '7', 0x00, '2', 0x00, '2', 0x00, 'C', 0x00, 'F', 0x00,
    'D', 0x00, '1', 0x00, '-', 0x00, '2', 0x00, '2', 0x00, 'D', 0x00, '2', 0x00,
    '-', 0x00, '4', 0x00, '1', 0x00, '0', 0x00, 'E', 0x00, '-', 0x00, '8', 0x00,
    'D', 0x00, '1', 0x00, '2', 0x00, '-', 0x00, '3', 0x00, '7', 0x00, '8', 0x00,
    '7', 0x00, '0', 0x00, '2', 0x00, '8', 0x00, '8', 0x00, '1', 0x00, '1', 0x00,
    '1', 0x00, '7', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00,

    // Function subset header length
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION_LEN),
    // Function subset header descriptor type
    U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION),
    // Second interface
    ITF_NUM_VENDOR,
    // Reserved
    0x00,
    // Length of this function subset including this header
    U16_TO_U8S_LE(MS_OS_20_SUBSET_FUNCTION_LEN),

    // Feature descriptor length
    U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID_LEN),
    // Feature descriptor type
    U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID),
    // Compatible ID
    // We use WinUSB to install generic USB driver for vendor interface
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
    // Sub-compatible ID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // Registry property descriptor length
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY_LEN),
    // Registry property descriptor type
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
    // Property data type (REG_MULTI_SZ)
    U16_TO_U8S_LE(0x0007),
    // Property name length
    U16_TO_U8S_LE(0x002A),
    // Property name
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00,
    'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00,
    'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00,
    0x00,
    // Property data length
    U16_TO_U8S_LE(0x0050),
    // Property data (randomly generated GUID)
    '{', 0x00, '0', 0x00, '5', 0x00, '7', 0x00, '0', 0x00, '4', 0x00, 'D', 0x00,
    'C', 0x00, '6', 0x00, '-', 0x00, '5', 0x00, '3', 0x00, '8', 0x00, 'C', 0x00,
    '-', 0x00, '4', 0x00, 'E', 0x00, '0', 0x00, '3', 0x00, '-', 0x00, '9', 0x00,
    '1', 0x00, 'B', 0x00, 'C', 0x00, '-', 0x00, 'D', 0x00, 'A', 0x00, 'F', 0x00,
    '2', 0x00, 'C', 0x00, '5', 0x00, '4', 0x00, '1', 0x00, '4', 0x00, 'D', 0x00,
    '9', 0x00, '1', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00

};

TU_VERIFY_STATIC(sizeof(desc_ms_os_20) == MS_OS_20_DESC_LEN, "Incorrect size");

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// String Descriptor Index
enum {
  STRID_LANGID = 0,
  STRID_MANUFACTURER,
  STRID_PRODUCT,
  STRID_SERIAL,
};

// array of pointer to string descriptors
char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    USB_MANUFACTURER_NAME,      // 1: Manufacturer
    USB_PRODUCT_NAME,           // 2: Product
    NULL,                       // 3: Serials will use unique ID if possible
    USB_VENDOR_INTF_NAME,       // 4: Vendor interface
};

static uint16_t _desc_str[32 + 1];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  size_t chr_count;

  switch (index) {
  case STRID_LANGID:
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
    break;

  case STRID_SERIAL:
    chr_count = board_usb_get_serial(_desc_str + 1, 32);
    break;

  default:
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-os-1-0-descriptors-specification

    if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
      return NULL;

    const char *str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    size_t const max_count =
        sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
    if (chr_count > max_count)
      chr_count = max_count;

    // Convert ASCII string into UTF-16
    for (size_t i = 0; i < chr_count; i++) {
      _desc_str[1 + i] = str[i];
    }
    break;
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

  return _desc_str;
}
