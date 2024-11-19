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

    .nkro = 1,
    .unused = 0,

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
                    TG(2),
                    KC_VOLU,
                    KC_LEFT,
                    KC_DOWN,
                    KC_RGHT,
                    KC_VOLD,
                },
                {
                    _______,
                    KC_PSCR,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
                    _______,
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
