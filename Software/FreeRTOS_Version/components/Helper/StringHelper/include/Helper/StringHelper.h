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
 * @file StringHelper.h
 * @brief String Helper Functions (header)
 *
 * @details This file contains the header information for the String Helper Functions
 *
 * @author Andrew Gawlik
 * @date May 10 2021
 *
 */
#include <stdint.h>
#include <stddef.h>

/**
 * @brief inserts an unsigned 8-bit integer into a string
 * @details inserts an unsigned 32-bit integer into a string at the location specified
 * @param[in] dest the destination string to insert the number into
 * @param[in] len, the length of the destination string
 * @param[in] loc, the index of the string to store the data into
 * @param[in] num, the number to be inserted into the string
 * @param[in] digits, the maximum number of digits to be inserted into the string (32-bit allows for 10 digits)
 * @return none
 *
 */
void helper_string_insert_uint32(char* dest, size_t len, size_t loc, uint32_t num, uint8_t digits);
