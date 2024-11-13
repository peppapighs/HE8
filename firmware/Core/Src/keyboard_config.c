/*
 * keyboard_config.c
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "keyboard_config.h"

#include "keyboard.h"
#include "keycodes.h"

keyboard_config_t keyboard_config = {
    .magic = KEYBOARD_CONFIG_MAGIC,
    .version = KEYBOARD_CONFIG_VERSION,

    .switch_profile = SWITCH_PROF_GEON_RAW_HE,
    .keyboard_profile = 0,

    .key_switch_config =
        {{CONFIG_RT(200, 10, 10, 10), CONFIG_RT(200, 10, 10, 10),
          CONFIG_RT(20, 20, 10, 10), CONFIG_RT(20, 20, 10, 10), CONFIG_ACT(200),
          CONFIG_ACT(200), CONFIG_ACT(200), CONFIG_ACT(200)},
         {CONFIG_RT(200, 10, 10, 10), CONFIG_RT(200, 10, 10, 10),
          CONFIG_RT(20, 20, 10, 10), CONFIG_RT(20, 20, 10, 10), CONFIG_ACT(200),
          CONFIG_ACT(200), CONFIG_ACT(200), CONFIG_ACT(200)}},

    .keymap = {
        {
            {KC_Z, KC_X, KC_C, KC_V, MO(1), DF(2), KC_VOLD, KC_VOLU},
            {KC_EXLM, KC_AT, KC_HASH, KC_DLR, _______, PS(1), _______, _______},
            {KC_Q, KC_W, KC_E, KC_R, MO(3), DF(0), KC_MUTE, KC_WAKE},
            {KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, _______, _______, _______,
             _______},
        },
        {
            {KC_A, KC_S, KC_D, KC_F, MO(1), DF(2), KC_VOLD, KC_VOLU},
            {KC_EXLM, KC_AT, KC_HASH, KC_DLR, _______, PS(0), _______, _______},
            {KC_N, KC_M, KC_COMM, KC_DOT, MO(3), DF(0), KC_MUTE, KC_WAKE},
            {KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, _______, _______, _______,
             _______},
        }}};
