/*
 * keyboard_config.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Pep
 */

#ifndef INC_KEYBOARD_CONFIG_H_
#define INC_KEYBOARD_CONFIG_H_

#include "keyboard.h"

//--------------------------------------------------------------------+
// Default Configurations
//--------------------------------------------------------------------+

extern keyboard_config_t const default_keyboard_config;

//--------------------------------------------------------------------+
// Switch Configuration Macros
//--------------------------------------------------------------------+

#define CONFIG_ACT(act)                                                        \
  {                                                                            \
    .tapping_term = 200, .mode = KEY_MODE_ACTUATION, .actuation = {            \
      .actuation_distance = (act),                                             \
    }                                                                          \
  }

#define CONFIG_RT(act, rst, rt_down, rt_up)                                    \
  {                                                                            \
    .tapping_term = 200, .mode = KEY_MODE_RAPID_TRIGGER, .rapid_trigger = {    \
      .actuation_distance = (act),                                             \
      .reset_distance = (rst),                                                 \
      .rt_down_distance = (rt_down),                                           \
      .rt_up_distance = (rt_up),                                               \
    }                                                                          \
  }

#endif /* INC_KEYBOARD_CONFIG_H_ */
