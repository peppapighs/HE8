/*
 * keyboard_config.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Pep
 */

#ifndef INC_KEYBOARD_CONFIG_H_
#define INC_KEYBOARD_CONFIG_H_

#include "keyboard.h"

static keyboard_config_t keyboard_config = {
    .magic = KEYBOARD_CONFIG_MAGIC,
    .version = KEYBOARD_CONFIG_VERSION,

    .switch_profile = SWITCH_PROF_GEON_RAW_HE,

    .key_switch_config =
        {{.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .reset_distance = 20,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
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
        {KC_Z, KC_X, KC_C, KC_V, KC_LEFT_CONTROL, KC_LEFT_SHIFT, KC_LEFT_ALT,
         KC_SPACEBAR},
    }}};

#endif /* INC_KEYBOARD_CONFIG_H_ */
