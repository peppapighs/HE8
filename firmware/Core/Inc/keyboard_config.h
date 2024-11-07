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
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_RAPID_TRIGGER,
          .rapid_trigger =
              {
                  .actuation_distance = 200,
                  .rt_down_distance = 10,
                  .rt_up_distance = 10,
              }},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION, .actuation = {.actuation_distance = 200}},
         {.mode = KEY_MODE_ACTUATION,
          .actuation = {.actuation_distance = 200}}},

    .keymap = {{{KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8}}}};

#endif /* INC_KEYBOARD_CONFIG_H_ */
