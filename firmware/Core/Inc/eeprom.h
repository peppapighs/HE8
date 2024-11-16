/*
 * eeprom.h
 *
 *  Created on: Nov 16, 2024
 *      Author: Pep
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//--------------------------------------------------------------------+
// EEPROM Functions
//--------------------------------------------------------------------+

// Return `true` if read successfully
bool eeprom_read(uint32_t address, uint8_t *data, size_t size);
// Return `true` if write successfully
bool eeprom_write(uint32_t address, uint8_t *data, size_t size);

#endif /* INC_EEPROM_H_ */
