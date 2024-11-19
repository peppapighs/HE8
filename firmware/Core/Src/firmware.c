/*
 * firmware.c
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#include "firmware.h"

#include "debug.h"
#include "key_switch.h"
#include "keyboard.h"
#include "keycodes.h"
#include "main.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// Keyboard State
//--------------------------------------------------------------------+

static uint16_t layer_mask;
static uint8_t default_layer_num;

//--------------------------------------------------------------------+
// HID Related Data
//--------------------------------------------------------------------+

// If we should send a remote wakeup to the host
static bool should_remote_wakeup;

typedef struct {
  uint8_t keycode;
  // Modifier will also be derived from the keycode
  uint8_t modifiers;
} kb_mods_action_t;

// Keyboard (including consumer control and system control) & modifier actions
static uint8_t kb_mods_actions_count;
static kb_mods_action_t kb_mods_actions[MAX_ACTION_COUNT];

// Tap actions
static uint8_t tap_actions_count;
static kb_mods_action_t tap_actions[MAX_ACTION_COUNT];

enum {
  ACTION_LAYER_ON,
  ACTION_LAYER_OFF,
  ACTION_LAYER_TOGGLE,
  ACTION_LAYER_DEFAULT,
};

typedef struct {
  uint8_t action;
  uint8_t layer;
  uint8_t modifiers;
} layer_action_t;

// Layer actions
static uint8_t layer_actions_count;
static layer_action_t layer_actions[MAX_ACTION_COUNT];

enum {
  ACTION_PROFILE_SET,
};

typedef struct {
  uint8_t action;
  uint8_t profile;
} profile_action_t;

// Profile actions
static uint8_t profile_actions_count;
static profile_action_t profile_actions[MAX_ACTION_COUNT];

// Keyboard keycodes
static uint8_t keyboard_keycodes_count;
static hid_kb_nkro_report_t keyboard_report;

// System control keycode
static uint16_t system_control_keycode;
static uint16_t last_system_control_keycode;

// Consumer control keycode
static uint16_t consumer_control_keycode;
static uint16_t last_consumer_control_keycode;

// Keycodes swap buffers
// We swap the buffers every time we send a HID report
static uint8_t keycodes_buffer_idx;
static uint16_t keycodes_buffer[2][NUM_KEYS];

//--------------------------------------------------------------------+
// Prototypes
//--------------------------------------------------------------------+

// Initialize keyboard state before firmware starts
static void keyboard_state_init(void);
// Initialize HID related data before firmware starts
static void hid_data_init(void);
// Poll keyboard switches
static void keyboard_task(void);

//--------------------------------------------------------------------+
// Firmware Functions
//--------------------------------------------------------------------+

void firmware_init(void) {
#ifdef DEBUG
  debug_init();
#endif

  load_keyboard_config();

  start_calibrating_key_switches();
  keyboard_state_init();
  hid_data_init();

  HAL_ADC_Start_IT(&hadc1);

  while (is_calibrating_key_switches())
    ;

  tusb_init(BOARD_TUD_RHPORT, NULL);
}

void firmware_loop(void) {
  tud_task();

  // Must be after `tud_task()` in case the user recalibrates the key switches
  // using a vendor request
  while (is_calibrating_key_switches())
    ;

  if (tud_suspended() && should_remote_wakeup) {
    // If we are in suspend mode, wake up the host
    tud_remote_wakeup();
  } else if (tud_hid_ready()) {
    // Otherwise, start the HID report chain
    send_hid_report(keyboard_config.nkro ? REPORT_ID_KB_NKRO
                                         : REPORT_ID_KEYBOARD);
    // Poll key switches after sending the report to avoid latency
    keyboard_task();
  }
}

//--------------------------------------------------------------------+
// Initialization Functions
//--------------------------------------------------------------------+

static void keyboard_state_init(void) {
  layer_mask = 0;
  default_layer_num = 0;
}

static void hid_data_init(void) {
  should_remote_wakeup = false;

  kb_mods_actions_count = 0;
  tap_actions_count = 0;
  layer_actions_count = 0;
  profile_actions_count = 0;

  keyboard_keycodes_count = 0;
  memset(&keyboard_report, 0, sizeof(hid_kb_nkro_report_t));

  system_control_keycode = KC_NO;
  last_system_control_keycode = KC_NO;

  consumer_control_keycode = KC_NO;
  last_consumer_control_keycode = KC_NO;

  keycodes_buffer_idx = 0;
  memset(keycodes_buffer[0], KC_NO, 2 * NUM_KEYS);
}

//--------------------------------------------------------------------+
// Polling Functions
//--------------------------------------------------------------------+

void clear_hid_data(void) {
  should_remote_wakeup = false;

  kb_mods_actions_count = 0;
  tap_actions_count = 0;
  layer_actions_count = 0;
  profile_actions_count = 0;

  keyboard_keycodes_count = 0;
  memset(&keyboard_report, 0, sizeof(hid_kb_nkro_report_t));

  system_control_keycode = KC_NO;

  consumer_control_keycode = KC_NO;
}

uint8_t current_layer(void) {
  if (layer_mask == 0)
    return default_layer_num;

  return 31 - __builtin_clz(layer_mask);
}

uint16_t get_keycode(uint8_t key_index) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;
  uint8_t const layer_num = current_layer();

  if (keyboard_config.keymap[keyboard_profile][layer_num][key_index] != KC_TRNS)
    return keyboard_config.keymap[keyboard_profile][layer_num][key_index];

  // Unlike QMK, we don't find the highest active layer but fallback to the
  // default layer instead
  return keyboard_config.keymap[keyboard_profile][default_layer_num][key_index];
}

void add_kb_mods_action(uint8_t keycode, uint8_t modifiers) {
  if (kb_mods_actions_count >= MAX_ACTION_COUNT)
    return;

  kb_mods_actions[kb_mods_actions_count].keycode = keycode;
  kb_mods_actions[kb_mods_actions_count].modifiers = modifiers;
  kb_mods_actions_count++;
}

void add_tap_action(uint8_t keycode, uint8_t modifiers) {
  if (tap_actions_count >= MAX_ACTION_COUNT)
    return;

  tap_actions[tap_actions_count].keycode = keycode;
  tap_actions[tap_actions_count].modifiers = modifiers;
  tap_actions_count++;
}

void add_layer_action(uint8_t action, uint8_t layer, uint8_t modifiers) {
  if (layer_actions_count >= MAX_ACTION_COUNT)
    return;

  layer_actions[layer_actions_count].action = action;
  layer_actions[layer_actions_count].layer = layer;
  layer_actions[layer_actions_count].modifiers = modifiers;
  layer_actions_count++;
}

void add_profile_action(uint8_t action, uint8_t profile) {
  if (profile_actions_count >= MAX_ACTION_COUNT)
    return;

  profile_actions[profile_actions_count].action = action;
  profile_actions[profile_actions_count].profile = profile;
  profile_actions_count++;
}

void process_basic_keycode(uint8_t keycode) {
  if (IS_KEYBOARD_KEY(keycode)) {
    // Add the keycode to NKRO bitmap
    keyboard_report.bitmap[keycode >> 3] |= 1 << (keycode & 0x07);

    if (keyboard_keycodes_count < 6) {
      for (uint8_t i = 0; i < keyboard_keycodes_count; i++) {
        if (keyboard_report.keycodes[i] == keycode)
          // Skip if the keycode is already in the report
          return;
      }

      keyboard_report.keycodes[keyboard_keycodes_count] = keycode;
      keyboard_keycodes_count++;
    }

  } else if (IS_MODIFIER_KEY(keycode)) {
    keyboard_report.modifiers |= keycode_to_modifier(keycode);

  } else if (IS_SYSTEM_CONTROL_KEY(keycode)) {
    // Up to one system control keycode can be sent at a time
    if (system_control_keycode != KC_NO)
      return;

    system_control_keycode = keycode_to_system_control(keycode);

  } else if (IS_CONSUMER_CONTROL_KEY(keycode)) {
    // Up to one consumer control keycode can be sent at a time
    if (consumer_control_keycode != KC_NO)
      return;

    consumer_control_keycode = keycode_to_consumer_control(keycode);
  }
}

static void keyboard_task(void) {
  uint8_t const keyboard_profile = keyboard_config.keyboard_profile;

  uint16_t *current_buffer = keycodes_buffer[keycodes_buffer_idx];
  uint16_t *previous_buffer = keycodes_buffer[keycodes_buffer_idx ^ 1];

#ifdef DEBUG
  debug_counter_start();
#endif

  // Clear HID data before polling key switches
  clear_hid_data();

  // Queue the actions for each key switch
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    // Process the ADC value change for each key switch
    process_key(i);

    uint16_t keycode = get_keycode(i);

    // If at least one key is pressed, we should wake up the host
    should_remote_wakeup |= key_switches[i].pressed;

    if (key_switches[i].pressed) {
      // Must release the previous key first if the current key is different
      if (previous_buffer[i] != KC_NO) {
        if (IS_ONE_TIME_KC(previous_buffer[i])) {
          // One-time key should be released first before it can be processed
          // again
          current_buffer[i] = previous_buffer[i];
          continue;
        }

        keycode = previous_buffer[i];
      }
      current_buffer[i] = keycode;

      if (IS_BASIC_KEY(keycode)) {
        add_kb_mods_action(keycode, 0);

      } else if (IS_MOD_MASK(keycode)) {
        add_kb_mods_action(MM_KEY(keycode), MM_MODS(keycode));

      } else if (IS_MOD_TAP(keycode)) {
        uint16_t const tapping_term =
            keyboard_config.key_switch_config[keyboard_profile][i].tapping_term;

        if (key_switches[i].last_press_time + tapping_term > HAL_GetTick())
          continue;

        add_kb_mods_action(KC_NO, MT_MODS(keycode));

      } else if (IS_LAYER_TAP(keycode)) {
        uint16_t const tapping_term =
            keyboard_config.key_switch_config[keyboard_profile][i].tapping_term;

        if (key_switches[i].last_press_time + tapping_term > HAL_GetTick())
          continue;

        add_layer_action(ACTION_LAYER_ON, LT_LAYER(keycode), 0);
      } else if (IS_LAYER_MOD(keycode)) {
        add_layer_action(ACTION_LAYER_ON, LM_LAYER(keycode), LM_MODS(keycode));

      } else if (IS_LAYER_MOMENTARY(keycode)) {
        add_layer_action(ACTION_LAYER_ON, MO_LAYER(keycode), 0);

      } else if (IS_LAYER_DEFAULT(keycode)) {
        add_layer_action(ACTION_LAYER_DEFAULT, DF_LAYER(keycode), 0);

      } else if (IS_LAYER_TOGGLE(keycode)) {
        add_layer_action(ACTION_LAYER_TOGGLE, TG_LAYER(keycode), 0);

      } else if (IS_PROFILE_SET(keycode)) {
        add_profile_action(ACTION_PROFILE_SET, PS_PROFILE(keycode));
      }
    } else {
      current_buffer[i] = KC_NO;

      // If the key release has been processed, skip the key
      if (previous_buffer[i] == KC_NO)
        continue;

      // Release the last pressed key
      keycode = previous_buffer[i];
      if (IS_MOD_TAP(keycode)) {
        uint16_t const tapping_term =
            keyboard_config.key_switch_config[keyboard_profile][i].tapping_term;

        if (key_switches[i].last_press_time + tapping_term > HAL_GetTick())
          add_tap_action(MT_KEY(keycode), 0);

      } else if (IS_LAYER_TAP(keycode)) {
        uint16_t const tapping_term =
            keyboard_config.key_switch_config[keyboard_profile][i].tapping_term;

        if (key_switches[i].last_press_time + tapping_term > HAL_GetTick())
          add_tap_action(LT_KEY(keycode), 0);
        else
          add_layer_action(ACTION_LAYER_OFF, LT_LAYER(keycode), 0);

      } else if (IS_LAYER_MOD(keycode)) {
        add_layer_action(ACTION_LAYER_OFF, LM_LAYER(keycode), 0);

      } else if (IS_LAYER_MOMENTARY(keycode)) {
        add_layer_action(ACTION_LAYER_OFF, MO_LAYER(keycode), 0);
      }
    }
  }

  // Construct the report payload from the actions
  // We prioritize tap actions
  for (uint8_t i = 0; i < tap_actions_count; i++) {
    process_basic_keycode(tap_actions[i].keycode);
    keyboard_report.modifiers |= tap_actions[i].modifiers;
  }

  for (uint8_t i = 0; i < kb_mods_actions_count; i++) {
    process_basic_keycode(kb_mods_actions[i].keycode);
    keyboard_report.modifiers |= kb_mods_actions[i].modifiers;
  }

  for (uint8_t i = 0; i < layer_actions_count; i++) {
    switch (layer_actions[i].action) {
    case ACTION_LAYER_ON:
      layer_mask |= 1 << layer_actions[i].layer;
      keyboard_report.modifiers |= layer_actions[i].modifiers;
      break;

    case ACTION_LAYER_OFF:
      layer_mask &= ~(1 << layer_actions[i].layer);
      break;

    case ACTION_LAYER_TOGGLE:
      layer_mask ^= 1 << layer_actions[i].layer;
      break;

    case ACTION_LAYER_DEFAULT:
      default_layer_num = layer_actions[i].layer;
      break;

    default:
      break;
    }
  }

  for (uint8_t i = 0; i < profile_actions_count; i++) {
    switch (profile_actions[i].action) {
    case ACTION_PROFILE_SET:
      set_keyboard_profile(profile_actions[i].profile);
      // Reset the layer mask and default layer number when changing profiles
      layer_mask = 0;
      default_layer_num = 0;
      break;

    default:
      break;
    }
  }

  // Swap the keycodes buffer
  keycodes_buffer_idx ^= 1;

#ifdef DEBUG
  debug_info.keyboard_task_latency = debug_counter_stop();
#endif
}

//--------------------------------------------------------------------+
// HID Report Functions
//--------------------------------------------------------------------+

bool send_hid_report(uint8_t report_id) {
  switch (report_id) {
  case REPORT_ID_KEYBOARD:
    // We can use the NKRO report since the first 8 bytes are the same
    tud_hid_report(REPORT_ID_KEYBOARD, &keyboard_report,
                   sizeof(hid_keyboard_report_t));
    return true;

  case REPORT_ID_KB_NKRO:
    tud_hid_report(REPORT_ID_KB_NKRO, &keyboard_report,
                   sizeof(hid_kb_nkro_report_t));
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
