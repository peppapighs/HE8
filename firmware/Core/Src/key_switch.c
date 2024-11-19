/*
 * key_switch.c
 *
 *  Created on: Nov 15, 2024
 *      Author: Pep
 */

#include "key_switch.h"

#include "keyboard.h"
#include "main.h"

//--------------------------------------------------------------------+
// Switch State
//--------------------------------------------------------------------+

static uint32_t calibration_start_time;
static uint8_t current_mux_index;
key_state_t key_switches[NUM_KEYS];

//--------------------------------------------------------------------+
// Key Switch Functions
//--------------------------------------------------------------------+

void start_calibrating_key_switches(void) {
  uint16_t const adc_rest_value =
      switch_profiles[keyboard_config.switch_profile].adc_rest_value;

  current_mux_index = 0;

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    key_switches[i].min_value = ADC_MAX_VALUE;
    // Initialize with approximation of the resting position
    key_switches[i].max_value = adc_rest_value;
    key_switches[i].adc_value = 0;
    key_switches[i].distance = 0;
    key_switches[i].peek_distance = 0;
    key_switches[i].last_press_time = 0;
    key_switches[i].state = KEY_SWITCH_REST;
    key_switches[i].pressed = false;
  }

  calibration_start_time = HAL_GetTick();
}

bool is_calibrating_key_switches(void) {
  return calibration_start_time + CALIBRATION_TIME_MS > HAL_GetTick();
}

void calibrate_key_switch(uint8_t key_index) {
  uint16_t const adc_offset =
      switch_profiles[keyboard_config.switch_profile].adc_offset;

  key_state_t *key = &key_switches[key_index];

  if (key->adc_value > key->max_value) {
    key->max_value = key->adc_value;

    if (key->adc_value >= adc_offset)
      key->min_value = key->adc_value - adc_offset;
    else
      key->min_value = 0;
  }
}

uint16_t adc_value_to_distance(uint8_t key_index) {
  uint32_t const switch_distance =
      switch_profiles[keyboard_config.switch_profile].travel_distance;

  key_state_t *key = &key_switches[key_index];

  if (key->adc_value >= key->max_value || key->min_value >= key->max_value)
    return 0;
  if (key->adc_value <= key->min_value)
    return switch_distance;

  // Normalize the ADC value to the distance table granularity
  uint16_t const normalized = (((uint32_t)(key->adc_value - key->min_value))
                               << DISTANCE_TABLE_GRAIN_LOG2) /
                              (key->max_value - key->min_value);

  return (switch_distance * distance_table[normalized]) >>
         DISTANCE_TABLE_GRAIN_LOG2;
}

void process_actuation(uint8_t key_index) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;
  uint16_t const actuation_distance =
      keyboard_config.key_switch_config[keyboard_profile][key_index]
          .actuation.actuation_distance;

  key_state_t *key = &key_switches[key_index];

  key->pressed = key->distance >= actuation_distance;
}

void process_rapid_trigger(uint8_t key_index) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;
  uint16_t const actuation_distance =
      keyboard_config.key_switch_config[keyboard_profile][key_index]
          .rapid_trigger.actuation_distance;
  uint16_t const reset_distance =
      keyboard_config.key_switch_config[keyboard_profile][key_index]
          .rapid_trigger.reset_distance;
  uint16_t const rt_down_distance =
      keyboard_config.key_switch_config[keyboard_profile][key_index]
          .rapid_trigger.rt_down_distance;
  uint16_t const rt_up_distance =
      keyboard_config.key_switch_config[keyboard_profile][key_index]
          .rapid_trigger.rt_up_distance;

  key_state_t *key = &key_switches[key_index];

  switch (key->state) {
  case KEY_SWITCH_REST:
    if (key->distance > actuation_distance) {
      // The key has reached the actuation distance so it is pressed
      key->state = KEY_SWITCH_RT_DOWN;
      key->peek_distance = key->distance;
      key->pressed = true;
    }
    break;

  case KEY_SWITCH_RT_DOWN:
    if (key->distance <= reset_distance) {
      // The key is below the reset distance so it is released
      key->state = KEY_SWITCH_REST;
      key->peek_distance = key->distance;
      key->pressed = false;
    } else if (key->distance + rt_up_distance < key->peek_distance) {
      // The key is released from the peek distance so it is released
      key->state = KEY_SWITCH_RT_UP;
      key->peek_distance = key->distance;
      key->pressed = false;
    } else if (key->distance > key->peek_distance) {
      // Update peek distance if needed
      key->peek_distance = key->distance;
    }
    break;

  case KEY_SWITCH_RT_UP:
    if (key->distance <= reset_distance) {
      // The key is below the reset distance so it is released
      key->state = KEY_SWITCH_REST;
      key->peek_distance = key->distance;
      key->pressed = false;
    } else if (key->peek_distance + rt_down_distance < key->distance) {
      // The key is pressed from the peek distance so it is pressed
      key->state = KEY_SWITCH_RT_DOWN;
      key->peek_distance = key->distance;
      key->pressed = true;
    } else if (key->distance < key->peek_distance) {
      // Update peek distance if needed
      key->peek_distance = key->distance;
    }
    break;

  default:
    break;
  }
}

void process_key(uint8_t key_index) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;

  key_state_t *key = &key_switches[key_index];
  bool pressed = key->pressed;

  // Calibrating min value
  if (key->adc_value < key->min_value)
    key->min_value = key->adc_value;

  key->distance = adc_value_to_distance(key_index);
  switch (keyboard_config.key_switch_config[keyboard_profile][key_index].mode) {
  case KEY_MODE_ACTUATION:
    process_actuation(key_index);
    break;

  case KEY_MODE_RAPID_TRIGGER:
    process_rapid_trigger(key_index);
    break;

  default:
    break;
  }

  // If the key is pressed, update the last press time
  if (!pressed && key->pressed)
    key->last_press_time = HAL_GetTick();
}

//--------------------------------------------------------------------+
// HAL Callbacks
//--------------------------------------------------------------------+

void update_key_switch(uint8_t key_index, uint16_t adc_value) {
  key_state_t *key = &key_switches[key_index];

  // Exponential smoothing
  key->adc_value =
      ((uint32_t)key->adc_value * ((1 << ADC_EXP_RSHIFT) - 1) + adc_value) >>
      ADC_EXP_RSHIFT;

  if (is_calibrating_key_switches())
    calibrate_key_switch(key_index);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc == &hadc1) {
    uint16_t adc_value = HAL_ADC_GetValue(&hadc1);

    for (uint8_t i = 0; i < NUM_MUX; i++)
      update_key_switch(mux_matrices[i][current_mux_index], adc_value);

    current_mux_index = (current_mux_index + 1) & (NUM_KEYS_PER_MUX - 1);
    for (uint8_t j = 0; j < NUM_MUX_SELECT_PINS; j++)
      HAL_GPIO_WritePin(mux_select_ports[j], mux_select_pins[j],
                        (GPIO_PinState)((current_mux_index >> j) & 1));

    HAL_TIM_Base_Start_IT(&htim10);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim10)
    HAL_ADC_Start_IT(&hadc1);
}
