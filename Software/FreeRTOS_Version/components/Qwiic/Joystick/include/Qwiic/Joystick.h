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
 * @file Joystick.h
 * @brief Qwiic I2C Joystick Driver (header)
 *
 * @details This file contains the header information for the Qwiic Joystick Driver using I2C (SparkFun Qwiic Joystick -- COM-15168)
 *
 * @author Andrew Gawlik
 * @date May 3 2021
 *
 */

#include <stdint.h>
#include "esp_err.h"

/**
 * @brief Struct for storing qwiic joystick data
 * @details Data structure for storing the 10-bit ADC value from the horizontal, vertical, and select button
 *
 */ 
typedef struct
{
   uint16_t horz; /* @brief 10-bit ADC value for the horizontal position */
   uint16_t vert; /* @brief 10-bit ADC value for the vertical position */
   uint8_t  select; /* @brief status of select button (pressed = 1, not pressed = 0) */
} QwiicJoystickType;


/**
 * @brief initializes Qwiic Joystick
 * @details clears the status register of the Qwiic Joystick Select button for initialization
 * @return none
 *
 */
void qwiic_joystick_init(void);

/**
 * @brief Read the horizontal position of the qwiic joystick
 * @details Reads the 10-bit ADC value from the horizontal position of the qwiic joystick
 * @param[out] xValue - address to write the 10-bit number 0-1023 identifying the horizontal position of the qwiic joystick
 * @return error code for I2C communication
 *
 */
esp_err_t qwiic_joystick_get_horz(uint16_t *xValue);

/**
 * @brief Read the vertical position of the qwiic joystick
 * @details Reads the 10-bit ADC value from the vertical position of the qwiic joystick
 * @param[out] yValue - address to write the 10-bit number 0-1023 identifying the vertical position of the qwiic joystick
 * @return error code for I2C communication
 *
 */
esp_err_t qwiic_joystick_get_vert(uint16_t *yValue);

/**
 * @brief Read current state of the button on the qwiic joystick
 * @details If the button is currently pressed, this will be set
 * @param[out] selCurrent - address to write the current register (1 if select button is currently pressed, 0 if not pressed)
 * @return error code for I2C communication
 *
 */
esp_err_t qwiic_joystick_get_select(uint8_t *selCurrent);

/**
 * @brief Check status of the button since last check on the qwiic joystick and notify that it has been read
 * @details If the button has been pressed since the last read, this will be set
 * @param[out] selStatus - address to write the status register (1 if select button was pressed since last read, 0 if not pressed since last read)
 * @return error code for I2C communication
 *
 */
esp_err_t qwiic_joystick_chk_select(uint8_t *selStatus);

/**
 * @brief Read all parameters from the qwiic joystick
 * @details Reads the horizontal, vertical, and status of select button from the qwiic joystick breakout board
 * @param[out] value - address of a struct to write the qwiic joystick data to
 * @return none
 *
 */
void qwiic_joystick_read(QwiicJoystickType* value);

/**
 * @brief Read the firmware version from the qwiic joystick
 * @details Reads the major and minor version of the qwiic joystick firmware
 * @param[out] major - address to write the qwiic joystick major firmware version number to
 * @param[out] minor - address to write the qwiic joystick minor firmware version number to
 * @return none
 *
 */
void qwiic_joystick_version(uint8_t *major, uint8_t *minor);

uint8_t qwiic_joystick_isconnected();
