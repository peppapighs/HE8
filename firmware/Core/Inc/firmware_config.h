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

// Switch distance in units of 0.01mm
#define SWITCH_DISTANCE 340 // Geon RAW HE
// #define SWITCH_DISTANCE 350 // Gateron Magnetic Jade
// #define SWITCH_DISTANCE 400 // Wooting Lekker/Geon Raptor HE
// #define SWITCH_DISTANCE 410 // Gateron KS-20
#define SWITCH_ACTUATION_DISTANCE 20
#define SWITCH_RT_DOWN_DISTANCE 20
#define SWITCH_RT_UP_DISTANCE 20

// Number of rounds to calibrate max ADC value
#define CALIBRATION_ROUNDS 10
// Lower bound for the range of ADC values
#define ADC_LOWER_BOUND 600

// Number of keys. Also represents unconnected MUX inputs
#define NUM_KEYS 8

#define NUM_MUX 1
#define NUM_MUX_SELECT_PINS 3
#define NUM_KEYS_PER_MUX (1 << NUM_MUX_SELECT_PINS)

// GPIO pins for MUX select
static GPIO_TypeDef *const mux_select_ports[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_GPIO_Port, MUX_SELECT_1_GPIO_Port, MUX_SELECT_2_GPIO_Port};
static uint16_t const mux_select_pins[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_Pin, MUX_SELECT_1_Pin, MUX_SELECT_2_Pin};

// Key matrix
static uint8_t const mux_matrices[NUM_MUX][NUM_KEYS_PER_MUX] = {
    {1, 2, 3, 0, 7, 4, 6, 5}};

// Keymap
static uint8_t const keymap[NUM_KEYS] = {KC_1, KC_2, KC_3, KC_4,

                                         KC_5, KC_6, KC_7, KC_8};

#endif /* INC_FIRMWARE_CONFIG_H_ */
