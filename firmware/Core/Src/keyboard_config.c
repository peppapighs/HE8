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
        {{MO(1), KC_UP, KC_MUTE, KC_VOLU, KC_LEFT, KC_DOWN, KC_RGHT, KC_VOLD},
         {_______, KC_PSCR, _______, _______, _______, _______, _______,
          _______},
         {_______, _______, _______, _______, _______, _______, _______,
          _______},
         {_______, _______, _______, _______, _______, _______, _______,
          _______}},
        {{_______, _______, _______, _______, _______, _______, _______,
          _______},
         {_______, _______, _______, _______, _______, _______, _______,
          _______},
         {_______, _______, _______, _______, _______, _______, _______,
          _______},
         {_______, _______, _______, _______, _______, _______, _______,
          _______}}}};
