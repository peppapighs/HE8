/*
 * firmware.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "firmware.h"

#include "tusb.h"

#include "keyboard_config.h"
#include "usb_descriptors.h"

enum {
  KEY_SWITCH_REST,
  KEY_SWITCH_RT_DOWN,
  KEY_SWITCH_RT_UP,
};

typedef struct {
  // Calibration data
  uint16_t min_value;
  uint16_t max_value;

  // Current state
  uint16_t adc_value;
  uint16_t distance;
  uint16_t peek_distance;
  uint8_t state;
  bool pressed;
} __attribute__((packed)) key_state_t;

static uint8_t calibration_round;
static uint8_t current_mux_index;
static key_state_t key_switches[NUM_KEYS];

// Initialize key switches before firmware starts
static void key_switch_init(void);
// Keyboard task
static void keyboard_task(void);
// Calibrate key switch during calibration rounds
static void calibrate_key_switch(uint8_t key_index, uint16_t adc_value);
// Process ADC value change for a key switch
static void process_key(uint8_t key_index, uint16_t adc_value);

void firmware_init(void) {
  calibration_round = 0;
  current_mux_index = 0;
  key_switch_init();

  HAL_ADC_Start_IT(&hadc1);

  while (calibration_round < CALIBRATION_ROUNDS)
    ;

  tusb_init(1, NULL);
}

void firmware_loop(void) {
  tud_task();
  keyboard_task();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc == &hadc1) {
    uint16_t adc_value = HAL_ADC_GetValue(&hadc1);

    for (uint8_t i = 0; i < NUM_MUX; i++) {
      if (calibration_round < CALIBRATION_ROUNDS)
        calibrate_key_switch(mux_matrices[i][current_mux_index], adc_value);
      else
        process_key(mux_matrices[i][current_mux_index], adc_value);
    }

    current_mux_index = (current_mux_index + 1) & (NUM_KEYS_PER_MUX - 1);
    if (calibration_round < CALIBRATION_ROUNDS && current_mux_index == 0)
      calibration_round++;

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

static void key_switch_init(void) {
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    key_switches[i].min_value = 4095;
    key_switches[i].max_value = 0;
    key_switches[i].adc_value = 0;
    key_switches[i].state = KEY_SWITCH_REST;
    key_switches[i].distance = 0;
    key_switches[i].peek_distance = 0;
    key_switches[i].pressed = false;
  }
}

static void keyboard_task(void) {
  if (!tud_hid_ready())
    return;

  uint8_t keycodes[6] = {0};
  uint8_t modifier = 0;
  uint8_t keycodes_count = 0;

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (key_switches[i].pressed) {
      // TODO: Implement keymap layers
      uint16_t keycode = keyboard_config.keymap[0][0][i];

      if (!IS_MODIFIER(keycode)) {
        keycodes[keycodes_count] = keycode;
        keycodes_count++;
      } else {
        modifier |= GET_MODIFIER_FLAG(keycode);
      }

      if (keycodes_count == 6)
        break;
    }
  }

  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycodes);
}

static void calibrate_key_switch(uint8_t key_index, uint16_t adc_value) {
  uint16_t const adc_offset =
      switch_profiles[keyboard_config.switch_profile].adc_offset;

  key_state_t *key = &key_switches[key_index];

  key->adc_value = adc_value;

  if (key->adc_value > key->max_value) {
    key->max_value = key->adc_value;

    if (key->adc_value >= adc_offset)
      key->min_value = key->adc_value - adc_offset;
    else
      key->min_value = 0;
  }
}

uint16_t adc_value_to_distance(uint8_t key_index) {
  uint16_t const switch_distance =
      switch_profiles[keyboard_config.switch_profile].travel_distance;

  key_state_t *key = &key_switches[key_index];

  if (key->adc_value > key->max_value || key->min_value >= key->max_value)
    return 0;
  if (key->adc_value < key->min_value)
    return switch_distance;

  // Linear interpolation
  uint32_t delta = key->max_value - key->adc_value;
  uint32_t range = key->max_value - key->min_value;

  return delta * switch_distance / range;
}

void process_actuation(uint8_t key_index) {
  uint16_t const actuation_distance =
      keyboard_config.key_switch_config[key_index].actuation.actuation_distance;

  key_state_t *key = &key_switches[key_index];

  key->pressed = key->distance >= actuation_distance;
}

void process_rapid_trigger(uint8_t key_index) {
  uint16_t const actuation_distance =
      keyboard_config.key_switch_config[key_index]
          .rapid_trigger.actuation_distance;
  uint16_t const reset_distance =
      keyboard_config.key_switch_config[key_index].rapid_trigger.reset_distance;
  uint16_t const rt_down_distance = keyboard_config.key_switch_config[key_index]
                                        .rapid_trigger.rt_down_distance;
  uint16_t const rt_up_distance =
      keyboard_config.key_switch_config[key_index].rapid_trigger.rt_up_distance;

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

static void process_key(uint8_t key_index, uint16_t adc_value) {
  key_state_t *key = &key_switches[key_index];

  // Exponential smoothing
  key->adc_value =
      ((uint32_t)key->adc_value * ((1 << ADC_EXP_RSHIFT) - 1) + adc_value) >>
      ADC_EXP_RSHIFT;

  // Calibrating min value
  if (key->adc_value < key->min_value)
    key->min_value = key->adc_value;

  key->distance = adc_value_to_distance(key_index);
  switch (keyboard_config.key_switch_config[key_index].mode) {
  case KEY_MODE_ACTUATION:
    process_actuation(key_index);
    break;

  case KEY_MODE_RAPID_TRIGGER:
    process_rapid_trigger(key_index);
    break;

  default:
    break;
  }
}
