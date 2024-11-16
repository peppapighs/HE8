/*
 * eeprom.c
 *
 *  Created on: Nov 16, 2024
 *      Author: Pep
 */

#include "eeprom.h"

#include "firmware_config.h"
#include "main.h"
#include "stm32f4xx_hal.h"

//--------------------------------------------------------------------+
// EEPROM Functions
//--------------------------------------------------------------------+

bool eeprom_read(uint32_t address, uint8_t *data, size_t size) {
  return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, address, EEPROM_ADDR_SIZE, data,
                          size, EEPROM_TIMEOUT_MS) == HAL_OK;
}

bool eeprom_write(uint32_t address, uint8_t *data, size_t size) {
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
