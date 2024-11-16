/*
 * keyboard.h
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "keyboard.h"

#include <memory.h>
#include <stdbool.h>

#include "eeprom.h"
#include "keyboard_config.h"
#include "main.h"

//--------------------------------------------------------------------+
// Keyboard Configuration
//--------------------------------------------------------------------+

switch_profile_t const switch_profiles[SWITCH_PROF_COUNT] = {
    [SWITCH_PROF_GEON_RAW_HE] = {.travel_distance = 340, .adc_offset = 700},
    [SWITCH_PROF_GATERON_MAGNETIC_JADE] = {.travel_distance = 350,
                                           .adc_offset = 700},
    [SWITCH_PROF_WOOTING_LEKKER] = {.travel_distance = 400, .adc_offset = 700},
    [SWITCH_PROF_GATERON_KS_20] = {.travel_distance = 410, .adc_offset = 700}};

keyboard_config_t keyboard_config;

//--------------------------------------------------------------------+
// Configuration Functions
//--------------------------------------------------------------------+

void load_keyboard_config(void) {
  uint32_t magic = 0;
  uint16_t version = 0;

  // Load default configuration
  memcpy(&keyboard_config, &default_keyboard_config, sizeof(keyboard_config));

  // TODO: Uncomment this when properly implementing EEPROM functions
  // // Read the magic and version from EEPROM
  // eeprom_read(offsetof(keyboard_config_t, magic), (uint8_t *)&magic,
  //             sizeof(magic));
  // eeprom_read(offsetof(keyboard_config_t, version), (uint8_t *)&version,
  //             sizeof(version));

  // if (magic != KEYBOARD_CONFIG_MAGIC) {
  //   // Set default configuration
  //   eeprom_write(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  // } else if (version != KEYBOARD_CONFIG_VERSION) {
  //   // FIXME: Handle version migration
  //   eeprom_write(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  // } else {
  //   eeprom_read(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  // }
}

void set_keyboard_profile(uint8_t profile) {
  // Check if the profile is valid
  if (profile >= NUM_PROFILES)
    return;

  keyboard_config.keyboard_profile = profile;

  // Save the new keymap profile
  eeprom_write(offsetof(keyboard_config_t, keyboard_profile),
               (uint8_t *)&keyboard_config.keyboard_profile,
               sizeof(keyboard_config.keyboard_profile));
}
