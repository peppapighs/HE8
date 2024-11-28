/*
 * keyboard_config.c
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "keyboard_config.h"

#include "keyboard.h"
#include "keycodes.h"

//--------------------------------------------------------------------+
// Default Configurations
//--------------------------------------------------------------------+

keyboard_config_t const default_keyboard_config = {
    .magic = FIRMWARE_MAGIC,
    .version = FIRMWARE_VERSION,

    .nkro = true,

    .switch_profile = SWITCH_PROF_GEON_RAW_HE,
    .keyboard_profile = 0,

    .key_switch_config =
        {
            {
                CONFIG_ACT(200),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_ACT(200),
                CONFIG_ACT(200),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_ACT(200),
            },
            {
                CONFIG_ACT(200),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_ACT(200),
                CONFIG_ACT(200),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_RT(20, 20, 10, 10),
                CONFIG_ACT(200),
            },
        },

    .keymap =
        {
            {
                {
                    LT(1, KC_MUTE),
                    KC_UP,
                    LT(2, KC_MPLY),
                    KC_VOLU,
                    KC_LEFT,
                    KC_DOWN,
                    KC_RGHT,
                    KC_VOLD,
                },
                {
                    _______,
                    KC_PSCR,
                    FW_BOOTLOADER,
                    FW_REBOOT,
                    FW_FACTORY_RESET,
                    FW_NKRO_ON,
                    FW_NKRO_OFF,
                    FW_NKRO_TOGGLE,
                },
                {
                    KC_0,
                    KC_1,
                    _______,
                    KC_3,
                    KC_4,
                    KC_5,
                    KC_6,
                    KC_7,
                },
                {
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                },
            },
            {
                {
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                },
                {
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                },
                {
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                },
                {
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                },
            },
        },
};
