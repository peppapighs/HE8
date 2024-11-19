/*
 * firmware_config.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_FIRMWARE_CONFIG_H_
#define INC_FIRMWARE_CONFIG_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

//--------------------------------------------------------------------+
// USB Device Configuration
//--------------------------------------------------------------------+

#define USB_MANUFACTURER_NAME "peppapighs"
#define USB_PRODUCT_NAME "HE8"
#define USB_VENDOR_INTF_NAME "HE8"

#define USB_VENDOR_ID 0xBeef
#define USB_PRODUCT_ID 0xAb01

// Landing page
#define WEBUSB_URL "example.com"

#define VENDOR_BUFFER_SIZE 16384

_Static_assert(VENDOR_BUFFER_SIZE <= UINT16_MAX,
               "VENDOR_BUFFER_SIZE must be less than 65536");

//--------------------------------------------------------------------+
// Hardware Configuration
//--------------------------------------------------------------------+

#define NUM_MUX 1
#define NUM_MUX_SELECT_PINS 3
#define NUM_KEYS_PER_MUX (1 << NUM_MUX_SELECT_PINS)

// GPIO pins for MUX select
extern GPIO_TypeDef *const mux_select_ports[NUM_MUX_SELECT_PINS];
extern uint16_t const mux_select_pins[NUM_MUX_SELECT_PINS];

// M24C64 EEPROM configuration
#define EEPROM_ADDR 0xA0
#define EEPROM_ADDR_SIZE I2C_MEMADD_SIZE_16BIT
#define EEPROM_PAGE_SIZE 32
#define EEPROM_TIMEOUT_MS 1000
#define EEPROM_WRITE_WAIT_MS 5

//--------------------------------------------------------------------+
// Keyboard Configuration
//--------------------------------------------------------------------+

// Application should set the following boot flag at the end of the RAM and
// perform a software to access the bootloader
#define BOOT_FLAG 0xCC00FFEE

#define FIRMWARE_MAGIC 0xBEEFDEAD
#define FIRMWARE_VERSION 0x0100

// TinyUSB supports up to 6 keys per report
#define KEY_ROLL_OVER 6

#define ADC_MAX_VALUE 4095
// Calibration time
// The interval should be long enough that the maximum ADC value is stable
#define CALIBRATION_TIME_MS 200
// ADC exponential smoothing
// Higher the value, slower the response, but more stable
#define ADC_EXP_RSHIFT 4
// ADC-to-distance table granularity
#define DISTANCE_TABLE_GRAIN_LOG2 10
#define DISTANCE_TABLE_GRAIN (1 << DISTANCE_TABLE_GRAIN_LOG2)

// Number of keys. Also represents unconnected MUX inputs
#define NUM_KEYS 8

_Static_assert(NUM_KEYS <= UINT8_MAX, "NUM_KEYS must be less than 256");

#define NUM_PROFILES 2
#define NUM_LAYERS 4

_Static_assert(NUM_PROFILES <= 16, "NUM_KEYS must be no more than 16");
_Static_assert(NUM_LAYERS <= 16, "NUM_LAYERS must be no more than 16");

// Key matrix
extern uint8_t const mux_matrices[NUM_MUX][NUM_KEYS_PER_MUX];

#endif /* INC_FIRMWARE_CONFIG_H_ */
