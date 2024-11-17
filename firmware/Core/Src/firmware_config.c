/*
 * firmware_config.c
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "firmware_config.h"

#include "main.h"

//--------------------------------------------------------------------+
// Hardware Configuration
//--------------------------------------------------------------------+

// GPIO pins for MUX select
GPIO_TypeDef *const mux_select_ports[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_GPIO_Port,
    MUX_SELECT_1_GPIO_Port,
    MUX_SELECT_2_GPIO_Port,
};
uint16_t const mux_select_pins[NUM_MUX_SELECT_PINS] = {
    MUX_SELECT_0_Pin,
    MUX_SELECT_1_Pin,
    MUX_SELECT_2_Pin,
};

//--------------------------------------------------------------------+
// Keyboard Configuration
//--------------------------------------------------------------------+

// Key matrix
uint8_t const mux_matrices[NUM_MUX][NUM_KEYS_PER_MUX] = {
    {
        1,
        2,
        3,
        0,
        7,
        4,
        6,
        5,
    },
};
