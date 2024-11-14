/*
 * debug.h
 *
 *  Created on: Nov 14, 2024
 *      Author: Pep
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#ifdef DEBUG

#include <stdint.h>

//--------------------------------------------------------------------+
// Debug Information
//--------------------------------------------------------------------+

typedef struct {
  // Number of cycles to finish polling the key switches
  uint32_t keyboard_task_latency;
} debug_info_t;

extern debug_info_t debug_info;

//--------------------------------------------------------------------+
// Debug Functions
//--------------------------------------------------------------------+

// Initialize cycle counter and debug information
void debug_init(void);
// Start cycle counter
void debug_counter_start(void);
// Stop cycle counter and return the elapsed cycles
uint32_t debug_counter_stop(void);

#endif

#endif /* INC_DEBUG_H_ */
