/*
 * bootloader_config.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_BOOTLOADER_CONFIG_H_
#define INC_BOOTLOADER_CONFIG_H_

//--------------------------------------------------------------------+
// USB Device Configuration
//--------------------------------------------------------------------+

#define USB_MANUFACTURER_NAME "peppapighs"
#define USB_PRODUCT_NAME "HE8 Bootloader"
#define USB_DFU_MODE_NAME "HE8 DFU Mode"

#define USB_VENDOR_ID 0xBeef
#define USB_PRODUCT_ID 0xDf01

// Landing page
#define WEBUSB_URL "example.com"

//--------------------------------------------------------------------+
// Bootloader Configuration
//--------------------------------------------------------------------+

// Delay until the boot button is sampled
#define BUTTON_SAMPLE_DELAY_MS 100

// Application should set the following boot flag at the end of the RAM and
// perform a software to access the bootloader
#define BOOT_FLAG 0xCC00FFEE

// Firmware magic values locating after the application code
#define FW_MAGIC1 0xDEADBEEF
#define FW_MAGIC2 0xABCDDCBA

// Flash sector 5 is chosen since it has 128K bytes, which should be enough for
// the entire application code
#define FW_FLASH_SECTOR FLASH_SECTOR_5
#define FW_FLASH_SECTOR_SIZE 0x20000
#define FW_BASE_ADDRESS 0x8020000

#endif /* INC_BOOTLOADER_CONFIG_H_ */
