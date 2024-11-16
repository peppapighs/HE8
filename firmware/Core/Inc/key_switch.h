/*
 * key_switch.h
 *
 *  Created on: Nov 15, 2024
 *      Author: Pep
 */

#ifndef INC_KEY_SWITCH_H_
#define INC_KEY_SWITCH_H_

#include <stdbool.h>

#include "firmware_config.h"

//--------------------------------------------------------------------+
// Switch State
//--------------------------------------------------------------------+

enum {
  KEY_SWITCH_REST,
  KEY_SWITCH_RT_DOWN,
  KEY_SWITCH_RT_UP,
};

typedef struct {
  // Calibration data
  uint16_t min_value;
  uint16_t max_value;

  // Distance data
  uint16_t adc_value;
  uint16_t distance;
  uint16_t peek_distance;

  // Timestamp
  uint32_t last_press_time;

  // Current state
  uint8_t state;
  bool pressed;
} __attribute__((packed)) key_state_t;

extern key_state_t key_switches[NUM_KEYS];

//--------------------------------------------------------------------+
// Key Switch Functions
//--------------------------------------------------------------------+

// Initialize the key switch state
void key_switch_state_init(void);
// Return `true` if we are calibrating the key switches
bool is_calibrating_key_switches(void);
// Process ADC value change for a key switch
void process_key(uint8_t key_index);

#endif /* INC_KEY_SWITCH_H_ */
