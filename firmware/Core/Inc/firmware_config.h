/*
 * firmware_config.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_FIRMWARE_CONFIG_H_
#define INC_FIRMWARE_CONFIG_H_

#define USB_MANUFACTURER_NAME "peppapighs"
#define USB_PRODUCT_NAME "HE8"
#define USB_VENDOR_INTF_NAME "HE8"

#define USB_VENDOR_ID 0xBeef
#define USB_PRODUCT_ID 0xAb01

// Landing page
#define WEBUSB_URL "example.com"

// Application should set the following boot flag at the end of the RAM and
// perform a software to access the bootloader
#define BOOT_FLAG 0xCC00FFEE

#endif /* INC_FIRMWARE_CONFIG_H_ */
