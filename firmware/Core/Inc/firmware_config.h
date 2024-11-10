/*
 * firmware_config.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_FIRMWARE_CONFIG_H_
#define INC_FIRMWARE_CONFIG_H_

#include <stdint.h>

#include "keycodes.h"
#include "main.h"

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
// Keyboard Configuration
//--------------------------------------------------------------------+

#define KEYBOARD_CONFIG_MAGIC 0xBEEFDEAD
#define KEYBOARD_CONFIG_VERSION 0x0100

// Calibration rounds
#define CALIBRATION_ROUNDS 16
// ADC exponential smoothing
#define ADC_EXP_RSHIFT 4

// Number of keys. Also represents unconnected MUX inputs
#define NUM_KEYS 8

#define NUM_MUX 1
#define NUM_MUX_SELECT_PINS 3
#define NUM_KEYS_PER_MUX (1 << NUM_MUX_SELECT_PINS)

#define NUM_PROFILES 1
#define NUM_LAYERS 1

// GPIO pins for MUX select
static GPIO_TypeDef *const mux_select_ports[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_GPIO_Port, MUX_SELECT_1_GPIO_Port, MUX_SELECT_2_GPIO_Port};
static uint16_t const mux_select_pins[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_Pin, MUX_SELECT_1_Pin, MUX_SELECT_2_Pin};

// Key matrix
static uint8_t const mux_matrices[NUM_MUX][NUM_KEYS_PER_MUX] = {
    {1, 2, 3, 0, 7, 4, 6, 5}};

#endif /* INC_FIRMWARE_CONFIG_H_ */
