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
 * @file StringHelper.c
 * @brief String Helper (source)
 *
 * @details This file contains the source code for the String Helper Functions
 *
 * @author Andrew Gawlik
 * @date May 10 2021
 *
 */

#include "Helper/StringHelper.h"

#define UINT32_MAX_DIGITS_BASE10 10


//the decimal place values for base 10
static const uint32_t base10[] = {
1u,
10u,
100u,
1000u,
10000u,
100000u,
1000000u,
10000000u,
100000000u,
1000000000u
};


//insert an ascii converted number into the destination string with the maximum number of digits listed
void helper_string_insert_uint32(char* dest, size_t len, size_t loc, uint32_t num, uint8_t digits)
{
  size_t current_digit_index, current_location=loc;
  uint32_t remainder;
  uint8_t digit;
  if((loc+digits-1) < len) //check to see if the location and maximum number of digits will be within the string, do not perform the insertion if the string location, maximum digits and length do not allow
  {
    if(digits < UINT32_MAX_DIGITS_BASE10)
    {
      remainder = num;  //initially set the remainder to the number to be printed out
      
      for(current_digit_index = (digits-1); current_digit_index > 0; current_digit_index--) //start at the most significant decimal place :10^(digits-1) working down to 10^1
      {
        digit = remainder/base10[current_digit_index];  //divide remainder by the decimal place value to find the digit
        dest[current_location] = (digit>0)?(('0'+digit)):' '; //place the character matching with the digit (replace with space if 0)
        remainder %= base10[current_digit_index]; //set the remainder to the number excluding what has already been printed out
        current_location++;  //increment the location
      }
  
      dest[current_location] = ('0'+remainder); //the remainder should be a single digit
    }
  }
}
