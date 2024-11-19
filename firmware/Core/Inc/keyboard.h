/*
 * keyboard.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Pep
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "firmware_config.h"

//--------------------------------------------------------------------+
// Keyboard Configuration
//--------------------------------------------------------------------+

enum {
  SWITCH_PROF_GEON_RAW_HE = 0x00,
  SWITCH_PROF_GATERON_MAGNETIC_JADE = 0x01,
  SWITCH_PROF_WOOTING_LEKKER = 0x02,
  SWITCH_PROF_GATERON_KS_20 = 0x03,
  SWITCH_PROF_COUNT = 0x04,
};

typedef struct {
  // In units of 0.01mm
  uint16_t travel_distance;
  // ADC approximation value of the switch resting position
  uint16_t adc_rest_value;
  // ADC offset used during calibration
  uint16_t adc_offset;
} __attribute__((packed)) switch_profile_t;

extern switch_profile_t const switch_profiles[SWITCH_PROF_COUNT];

// Generated by `python distance_table.py DISTANCE_TABLE_GRAIN`
extern uint16_t const distance_table[DISTANCE_TABLE_GRAIN];

enum {
  KEY_MODE_ACTUATION = 0x0,
  KEY_MODE_RAPID_TRIGGER = 0x1,
};

typedef struct {
  // In units of 0.01mm
  uint16_t actuation_distance;
} __attribute__((packed)) key_mode_actuation_config_t;

typedef struct {
  // In units of 0.01mm
  uint16_t actuation_distance;
  uint16_t reset_distance;
  uint16_t rt_down_distance;
  uint16_t rt_up_distance;
} __attribute__((packed)) key_mode_rapid_trigger_config_t;

typedef struct {
  // In units of ms
  uint16_t tapping_term;
  uint8_t mode;

  union {
    key_mode_actuation_config_t actuation;
    key_mode_rapid_trigger_config_t rapid_trigger;
  };
} __attribute__((packed)) key_switch_config_t;

typedef struct {
  // Config header
  uint32_t magic;
  uint16_t version;

  // NKRO on/off
  uint8_t nkro : 1;
  uint8_t unused : 7;

  // Switch profile
  uint8_t switch_profile;
  // Current keyboard profile
  uint8_t keyboard_profile;
  // Key switch configuration
  key_switch_config_t key_switch_config[NUM_PROFILES][NUM_KEYS];
  // Keymap
  uint16_t keymap[NUM_PROFILES][NUM_LAYERS][NUM_KEYS];
} __attribute__((packed)) keyboard_config_t;

_Static_assert(sizeof(keyboard_config_t) <= UINT16_MAX,
               "keyboard_config_t size must be less than 65536");

extern keyboard_config_t keyboard_config;

//--------------------------------------------------------------------+
// Configuration Functions
//--------------------------------------------------------------------+

// Load keyboard configuration from EEPROM
// If the configuration is invalid, load default configuration and save it
// If the configuration version is outdated, migrate it
void load_keyboard_config(void);
// Save current keyboard configuration to EEPROM
void save_keyboard_config(void);
// Save current key switch configuration to EEPROM
void save_key_switch_config(uint8_t profile, uint8_t key_index);
// Save current keymap to EEPROM
void save_keymap(uint8_t profile, uint8_t layer, uint8_t key_index);
// Set switch profile
void set_switch_profile(uint8_t profile);
// Set keyboard profile
void set_keyboard_profile(uint8_t profile);

#endif /* INC_KEYBOARD_H_ */
