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
 * @file Qwiic/QwiicController.h
 * @brief Qwiic Controller Device Driver (header) using the ESP32 as a Controller
 *
 * @details This file contains the header information for the Qwiic Device Driver via I2C as a Controller
 *
 * @author Andrew Gawlik
 * @date May 3 2021
 *
 */

#include "esp_err.h"

/**
 * @brief initializes Qwiic System in Controller Mode
 * @details initializes the Qwiic connect port (and associated pins) in I2C controller mode
 * @return none
 *
 */ 
void qwiic_controller_init();

/**
 * @brief Write data to a peripheral via I2C/Qwiic System
 * @details Write data to a peripheral via I2C/Qwiic System
 * @param[in] peripheralAddress the address of the peripheral on the I2C bus (only 7-bit addresses will work)
 * @param[in] registerToWrite the address on the periperal to write to in memory
 * @param[in] dataToWrite the data to be written to memory on the peripheral
 * @param[in] ack_en enables the ack check for controller
 * @return none
 *
 */
esp_err_t qwiic_controller_write_to_peripheral(uint8_t peripheralAddress, uint8_t registerToWrite, uint8_t dataToWrite);

/**
 * @brief Read data from a peripheral via I2C/Qwiic System
 * @details Read data from a peripheral via I2C/Qwiic System
 * @param[in] peripheralAddress the address of the peripheral on the I2C bus (only 7-bit addresses will work)
 * @param[in] registerToRead the address on the periperal to read from in memory
 * @param[in] ack_en enables the ack check for controller
 * @return Data Read
 *
 */
esp_err_t qwiic_controller_read_from_peripheral(uint8_t peripheralAddress, uint8_t registerToRead, uint8_t *dataRead);


int qwiic_controller_get_timeout();

void qwiic_controller_set_timeout(int timeout);

uint8_t qwiic_controller_is_peripheral_connected(uint8_t peripheralAddress);
