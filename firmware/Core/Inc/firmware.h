/*
 * firmware.h
 *
 *  Created on: Nov 4, 2024
 *      Author: Pep
 */

#ifndef INC_FIRMWARE_H_
#define INC_FIRMWARE_H_

#include <stdbool.h>
#include <stdint.h>

//--------------------------------------------------------------------+
// Firmware Functions
//--------------------------------------------------------------------+

// Firmware initialization
void firmware_init(void);
// Firmware main loop
void firmware_loop(void);

//--------------------------------------------------------------------+
// HID Report Functions
//--------------------------------------------------------------------+

// Send a HID report
bool send_hid_report(uint8_t report_id);

#endif /* INC_FIRMWARE_H_ */
