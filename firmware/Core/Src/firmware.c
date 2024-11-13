/*
 * firmware.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "firmware.h"

#include "tusb.h"

#include "keyboard_config.h"
#include "keycodes.h"
#include "main.h"
#include "usb_descriptors.h"

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

//--------------------------------------------------------------------+
// Keyboard State
//--------------------------------------------------------------------+

static uint8_t layer_num;
static uint8_t default_layer_num;

//--------------------------------------------------------------------+
// HID Related Data
//--------------------------------------------------------------------+

static bool should_remote_wakeup;

// Keyboard keycodes
static uint8_t keyboard_keycodes_count;
static uint8_t keyboard_keycodes[KEY_ROLL_OVER];
static uint8_t keycodes_modifier;

// Keycodes swap buffer
static uint8_t keycodes_buffer_idx;
static uint16_t keycodes_buffer[2][NUM_KEYS];

// System control keycode
static uint16_t system_control_keycode;
static uint16_t last_system_control_keycode;

// Consumer control keycode
static uint16_t consumer_control_keycode;
static uint16_t last_consumer_control_keycode;

// Gamepad report
static hid_gamepad_report_t gamepad_report;

//--------------------------------------------------------------------+
// Prototypes
//--------------------------------------------------------------------+

// Initialize key switches before firmware starts
static void key_switch_state_init(void);
// Initialize keyboard state before firmware starts
static void keyboard_state_init(void);
// Initialize HID related data before firmware starts
static void hid_data_init(void);
// Poll keyboard switches
static void keyboard_task(void);
// Send HID report. Return `true` if the report is sent
static bool send_hid_report(uint8_t report_id);
// Calibrate key switch during calibration rounds
static void calibrate_key_switch(uint8_t key_index, uint16_t adc_value);
// Process ADC value change for a key switch
static void process_key(uint8_t key_index, uint16_t adc_value);

//--------------------------------------------------------------------+
// Implementation
//--------------------------------------------------------------------+

void firmware_init(void) {
  // TODO: Uncomment the following line after implementing EEPROM functions
  // load_keyboard_config();

  key_switch_state_init();
  keyboard_state_init();
  hid_data_init();

  HAL_ADC_Start_IT(&hadc1);

  while (calibration_round < CALIBRATION_ROUNDS)
    ;

  tusb_init(1, NULL);
}

void firmware_loop(void) {
  tud_task();
  keyboard_task();

  if (tud_suspended() && should_remote_wakeup)
    // If we are in suspend mode, wake up the host
    tud_remote_wakeup();
  else if (tud_hid_ready())
    // Otherwise, start the HID report chain
    send_hid_report(REPORT_ID_KEYBOARD);
}

//--------------------------------------------------------------------+
// HID Related Callbacks
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report,
                                uint16_t len) {
  for (uint8_t i = report[0] + 1; i < REPORT_ID_COUNT; i++) {
    if (send_hid_report(i))
      return;
  }
}

//--------------------------------------------------------------------+
// HAL Callbacks
//--------------------------------------------------------------------+

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

//--------------------------------------------------------------------+
// Helper Functions
//--------------------------------------------------------------------+

static void key_switch_state_init(void) {
  calibration_round = 0;
  current_mux_index = 0;

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    key_switches[i].min_value = ADC_MAX_VALUE;
    key_switches[i].max_value = 0;
    key_switches[i].adc_value = 0;
    key_switches[i].state = KEY_SWITCH_REST;
    key_switches[i].distance = 0;
    key_switches[i].peek_distance = 0;
    key_switches[i].pressed = false;
  }
}

static void keyboard_state_init(void) {
  layer_num = 0;
  default_layer_num = 0;
}

static void hid_data_init(void) {
  should_remote_wakeup = false;

  keyboard_keycodes_count = 0;
  memset(keyboard_keycodes, KC_NO, KEY_ROLL_OVER);
  keycodes_modifier = 0;

  keycodes_buffer_idx = 0;
  memset(keycodes_buffer[0], KC_NO, 2 * NUM_KEYS);

  system_control_keycode = KC_NO;
  last_system_control_keycode = KC_NO;

  consumer_control_keycode = KC_NO;
  last_consumer_control_keycode = KC_NO;

  gamepad_report = (hid_gamepad_report_t){0};
}

void clear_hid_data(void) {
  should_remote_wakeup = false;

  keyboard_keycodes_count = 0;
  memset(keyboard_keycodes, KC_NO, KEY_ROLL_OVER);
  keycodes_modifier = 0;

  system_control_keycode = KC_NO;

  consumer_control_keycode = KC_NO;

  gamepad_report = (hid_gamepad_report_t){0};
}

static void keyboard_task(void) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;
  uint16_t *current_keycodes_buffer = keycodes_buffer[keycodes_buffer_idx];
  uint16_t *previous_keycodes_buffer = keycodes_buffer[keycodes_buffer_idx ^ 1];

  // Clear HID data before polling key switches
  clear_hid_data();

  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    uint16_t keycode = keyboard_config.keymap[keyboard_profile][layer_num][i];
    if (keycode == KC_TRNS)
      keycode = keyboard_config.keymap[keyboard_profile][default_layer_num][i];

    // If at least one key is pressed, we should wake up the host
    should_remote_wakeup |= key_switches[i].pressed;

    // We must always update the keycodes buffer so we can avoid clearing the
    // buffer every time we poll the key switches
    current_keycodes_buffer[i] = key_switches[i].pressed ? keycode : KC_NO;

    if (key_switches[i].pressed) {
      // We cannot process the keycode if the previous keycode is a one-time key
      // and it is still pressed
      if (IS_ONE_TIME_KEY(previous_keycodes_buffer[i]))
        continue;

      if (IS_KEYBOARD_KEY(keycode)) {
        if (keyboard_keycodes_count >= KEY_ROLL_OVER)
          continue;

        keyboard_keycodes[keyboard_keycodes_count] = keycode;
        keyboard_keycodes_count++;

      } else if (IS_MODIFIER_KEY(keycode)) {
        keycodes_modifier |= keycode_to_modifier(keycode);

      } else if (IS_SYSTEM_CONTROL_KEY(keycode)) {
        system_control_keycode = keycode_to_system_control(keycode);

      } else if (IS_CONSUMER_CONTROL_KEY(keycode)) {
        consumer_control_keycode = keycode_to_consumer_control(keycode);

      } else if (IS_MOD_MASK(keycode)) {
        // Check the number of keycodes first so that we don't override the
        // modifier unless there is a buffer space for the keycode
        if (keyboard_keycodes_count >= KEY_ROLL_OVER)
          continue;

        keycodes_modifier |= MM_MODS(keycode);
        keyboard_keycodes[keyboard_keycodes_count] = MM_KEY(keycode);
        keyboard_keycodes_count++;

      } else if (IS_LAYER_MOMENTARY(keycode)) {
        layer_num = MO_LAYER(keycode);

      } else if (IS_LAYER_DEFAULT(keycode)) {
        if (layer_num == default_layer_num)
          layer_num = DF_LAYER(keycode);
        default_layer_num = DF_LAYER(keycode);

      } else if (IS_LAYER_TOGGLE(keycode)) {
        layer_num = layer_num == TG_LAYER(keycode) ? default_layer_num
                                                   : TG_LAYER(keycode);
      } else if (IS_PROFILE_SET(keycode)) {
        set_keyboard_profile(PS_PROFILE(keycode));

      } else {
        // TODO: Implement mouse and gamepad
      }
    } else {
      // If the momentary layer is released, revert to the default layer
      if (IS_LAYER_MOMENTARY(keycode) && layer_num == MO_LAYER(keycode))
        layer_num = default_layer_num;
    }
  }

  keycodes_buffer_idx ^= 1;
}

static bool send_hid_report(uint8_t report_id) {
  switch (report_id) {
  case REPORT_ID_KEYBOARD:
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, keycodes_modifier,
                            keyboard_keycodes);
    return true;

  case REPORT_ID_SYSTEM_CONTROL:
    // Prevent multiple reports of the same keycode
    if (system_control_keycode != last_system_control_keycode) {
      tud_hid_report(REPORT_ID_SYSTEM_CONTROL, &system_control_keycode,
                     sizeof(system_control_keycode));
      last_system_control_keycode = system_control_keycode;
      return true;
    }
    break;

  case REPORT_ID_CONSUMER_CONTROL:
    // Prevent multiple reports of the same keycode
    if (consumer_control_keycode != last_consumer_control_keycode) {
      tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &consumer_control_keycode,
                     sizeof(consumer_control_keycode));
      last_consumer_control_keycode = consumer_control_keycode;
      return true;
    }
    break;

  case REPORT_ID_MOUSE:
    // TODO: Implement mouse
    break;

  case REPORT_ID_GAMEPAD:
    // TODO: Implement gamepad
    break;

  default:
    break;
  }

  return false;
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

  // Quadratic interpolation
  uint32_t const numerator = (uint32_t)(key->adc_value - key->min_value) *
                             (key->adc_value - key->min_value) *
                             switch_distance;
  uint32_t const denominator = (uint32_t)(key->max_value - key->min_value) *
                               (key->max_value - key->min_value);

  return (uint32_t)switch_distance - numerator / denominator;
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

static void process_key(uint8_t key_index, uint16_t adc_value) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;

  key_state_t *key = &key_switches[key_index];

  // Exponential smoothing
  key->adc_value =
      ((uint32_t)key->adc_value * ((1 << ADC_EXP_RSHIFT) - 1) + adc_value) >>
      ADC_EXP_RSHIFT;

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
}
