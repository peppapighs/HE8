/*
 * keyboard_config.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Pep
 */

#ifndef INC_KEYBOARD_CONFIG_H_
#define INC_KEYBOARD_CONFIG_H_

#include "keyboard.h"
#include "keycodes.h"

static keyboard_config_t keyboard_config = {
    .magic = KEYBOARD_CONFIG_MAGIC,
    .version = KEYBOARD_CONFIG_VERSION,

    .switch_profile = SWITCH_PROF_GEON_RAW_HE,
    .keymap_profile = 0,

    .key_switch_config =
        {{.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .reset_distance = 10,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .reset_distance = 10,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 20,
                  .reset_distance = 20,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 20,
                  .reset_distance = 20,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION,
          .actuation = {.actuation_distance = 200}}},

    .keymap = {{
        {KC_Z, KC_X, KC_C, KC_V, MO(1), DF(2), KC_VOLD, KC_VOLU},
        {KC_A, KC_S, KC_D, KC_F, _______, _______, _______, _______},
        {KC_Q, KC_W, KC_E, KC_R, MO(3), DF(0), KC_MUTE, KC_WAKE},
        {KC_H, KC_J, KC_K, KC_L, _______, _______, _______, _______},
    }}};

#endif /* INC_KEYBOARD_CONFIG_H_ */
