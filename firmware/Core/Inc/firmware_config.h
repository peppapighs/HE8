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

// Application should set the following boot flag at the end of the RAM and
// perform a software to access the bootloader
#define BOOT_FLAG 0xCC00FFEE

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

#define KEYBOARD_CONFIG_MAGIC 0xBEEFDEAD
#define KEYBOARD_CONFIG_VERSION 0x0100

// TinyUSB supports up to 6 keys per report
#define KEY_ROLL_OVER 6

#define ADC_MAX_VALUE 4095
// Calibration rounds
#define CALIBRATION_ROUNDS 16
// ADC exponential smoothing
// Higher the value, slower the response, but more stable
#define ADC_EXP_RSHIFT 4
// The closer the value to 1024, the more quadratic the ADC-to-distance curve
#define ADC_MIN_MULTIPLIER 719

// Number of keys. Also represents unconnected MUX inputs
#define NUM_KEYS 8

#define NUM_PROFILES 2
#define NUM_LAYERS 4

// Key matrix
extern uint8_t const mux_matrices[NUM_MUX][NUM_KEYS_PER_MUX];

#endif /* INC_FIRMWARE_CONFIG_H_ */
