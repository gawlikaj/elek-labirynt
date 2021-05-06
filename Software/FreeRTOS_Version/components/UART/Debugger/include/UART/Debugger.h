/******************************************************************************
 * Copyright (C) 2021 by Andrew Gawlik
 *
 *     This file is part of elek-labirynt.
 *
 *   elek-labirynt is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   elek-labirynt is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with elek-labirynt.  If not, see <https://www.gnu.org/licenses/>.
 *
 *****************************************************************************/

/**
 * @file debugger.h
 * @brief UART Debugger Driver (header)
 *
 * @details This file contains the header information for the UART debugger driver
 *
 * @author Andrew Gawlik
 * @date May 3 2021
 *
 */
#include <stdint.h>
#include <stddef.h>

/**
 * @brief initializes UART debugger
 * @details Configures the UART, installs the driver, and sets the pins
 * @return none
 *
 */
void debug_init();

/**
 * @brief runs the debugger task 
 * @details outputs the debugging information from the debugging handles to the UART from the microcontroller
 * @return none
 *
 */ 
void debug_task();

/**
 * @brief generates a new debug handle
 * @details generates a new debug handle dedicated to the calling process
 * @return none
 *
 */
uint8_t debug_add_handle();

/**
 * @brief Send message to the debugger
 *
 * @param[in] handleIndex a number identifying the debugger message handle assigned for the process
 * @param[in] data a character string of data to output from the microcontroller via the UART
 * @param[in] number of characters in the character string to output from the microcontroller via the UART
 * @return none
 */
void debug_out(uint8_t handleIndex, char* data, size_t len);
