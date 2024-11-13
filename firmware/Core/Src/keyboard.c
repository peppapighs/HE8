/*
 * keyboard.h
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "keyboard.h"

#include <stdbool.h>

#include "keyboard_config.h"
#include "main.h"

//--------------------------------------------------------------------+
// Prototypes
//--------------------------------------------------------------------+

// Return `true` if read successfully
static bool eeprom_read(uint32_t address, uint8_t *data, size_t size);
// Return `true` if write successfully
static bool eeprom_write(uint32_t address, uint8_t *data, size_t size);

//--------------------------------------------------------------------+
// Implementation
//--------------------------------------------------------------------+

void load_keyboard_config(void) {
  uint32_t magic = 0;
  uint16_t version = 0;

  eeprom_read(offsetof(keyboard_config_t, magic), (uint8_t *)&magic,
              sizeof(magic));
  eeprom_read(offsetof(keyboard_config_t, version), (uint8_t *)&version,
              sizeof(version));

  if (magic != KEYBOARD_CONFIG_MAGIC) {
    // Set default configuration
    eeprom_write(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  } else if (version != KEYBOARD_CONFIG_VERSION) {
    // FIXME: Handle version migration
    eeprom_write(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  } else {
    eeprom_read(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  }
}

void set_keymap_profile(uint8_t profile) {
  // Check if the profile is valid
  if (profile >= NUM_PROFILES)
    return;

  keyboard_config.keymap_profile = profile;

  // Save the new keymap profile
  eeprom_write(offsetof(keyboard_config_t, keymap_profile),
               (uint8_t *)&keyboard_config.keymap_profile,
               sizeof(keyboard_config.keymap_profile));
}

//--------------------------------------------------------------------+
// Helper Functions
//--------------------------------------------------------------------+

static bool eeprom_read(uint32_t address, uint8_t *data, size_t size) {
  return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, address, EEPROM_ADDR_SIZE, data,
                          size, EEPROM_TIMEOUT_MS) == HAL_OK;
}

static bool eeprom_write(uint32_t address, uint8_t *data, size_t size) {
  while (size > 0) {
    size_t write_size = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE);

    if (write_size > size)
      write_size = size;

    if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, address, EEPROM_ADDR_SIZE, data,
                          write_size, EEPROM_TIMEOUT_MS) != HAL_OK)
      return false;

    // Wait for the EEPROM to finish writing
    HAL_Delay(EEPROM_WRITE_WAIT_MS);

    size -= write_size;
    address += write_size;
    data += write_size;
  }

  return true;
}
