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
 * @file buttons.h
 * @brief Momentary Pushbuttons Interface (header)
 *
 * @details This file contains the header information for the Momentary Pushbuttons using Free RTOS on the ESP32
 *
 * @author Andrew Gawlik
 * @date Jul 1 2021
 *
 */

#include "esp_err.h"

#ifndef BUTTONS_H
#define BUTTONS_H

/**
 * @brief enumeration of buttons available
 * @details there are only 5 buttons supported here, this is to ensure that there are no incorrect values sent in functions that require specification of a particular button
 */
typedef enum {BUTTON0=0,BUTTON1=1,BUTTON2=2,BUTTON3=3,BUTTON4=4} ButtonId;

/**
 *  @brief type definition for a button handler function pointer
 */
typedef void(*ButtonHandler)(void*);

/**
 *  @brief Initialize the GPIO settings for the momentary pushbuttons
 *  @details The GPIO configuration is performed ensuring that each GPIO pin is configured appropriately for the buttons and installs the interrupt service routine for GPIO
 *  @params none
 *  @return ESP_OK                    - if the interrupt service routine for GPIO installation was successful
 *          ESP_ERR_NO_MEM            - if no memory is available to install the service routine
 *          ESP_ERR_INVALID_STATE_ISR - the service routine is already installed
 *          ESP_ERR_NOT_FOUND         - no free interrupt found
 *          ESP_ERR_INVALID_ARG_GPIO  - invalid GPIO pins assigned in source
 */
esp_err_t buttons_init(void);

/**
 *  @brief Read a particular button
 *  @details Read the button status of a particular button
 *  @param[in] selButtonId - the button to be read (see the enumeration above)
 *  @return the state of the button as read from the input register of the GPIO
 */
uint8_t buttons_read(ButtonId selButtonId);

/**
 * @brief Add a handler to a particular button
 * @details Adds a function pointer with arguments as part of a handler routine for the specified button
 * @oaram[in] selButtonId   - the button to be assigned a handler
 * @param[in] buttonHandler - the function pointer for the handler to be used during the interrupt service routine
 * @param[in] args          - the arguments to be passed into the handler for processing (if not used pass in NULL)
 * @return ESP_OK                - if adding the button handler was successful
 *         ESP_ERR_INVALID_STATE - if the interrupt service routine has not been initialized (call buttons_init)
           ESP_ERR_INVALID_ARG   - if the GPIO pins selected in the source are invalid
 */
esp_err_t buttons_add_handler(ButtonId selButtonId, ButtonHandler buttonHandler, void* args);

/**
 * @brief Remove a handler from a particular button
 * @details Removes the button handler from a particular button along with its reference
 * @param[in] selButtonId - the button to have its handler removed
 * @return ESP_OK                - if removing the button handler was successful
 *         ESP_ERR_INVALID_STATE - if the interrupt service routine has not been initialized (call buttons_init)
 *         ESP_ERR_INVALID_ARG   - if the GPIO pins selected in the source are invalid
 */
esp_err_t buttons_remove_handler(ButtonId selButtonId);

/**
 *  @brief Uninstall the interrupt service routine
 *  @details Uninstall the interrupt service routine for interrupt handling
 *  @params none
 *  @return none
 */
void buttons_deinit(void);

/**
 *  @brief Parent function of handler for button0
 *  @details the Parent function of the handler for button0 including debouncing code
 *  @param vpParam - arguments to be passed into the interrupt service routine for button0
 *  @return none
 */
void button0_handler(void* vpParam);

/**
 *  @brief Parent function of handler for button1
 *  @details the Parent function of the handler for button1 including debouncing code
 *  @param vpParam - arguments to be passed into the interrupt service routine for button1
 *  @return none
 */
void button1_handler(void* vpParam);

/**
 *  @brief Parent function of handler for button2
 *  @details the Parent function of the handler for button2 including debouncing code
 *  @param vpParam - arguments to be passed into the interrupt service routine for button2
 *  @return none
 */
void button2_handler(void* vpParam);

/**
 *  @brief Parent function of handler for button3
 *  @details the Parent function of the handler for button3 including debouncing code
 *  @param vpParam - arguments to be passed into the interrupt service routine for button3
 *  @return none
 */
void button3_handler(void* vpParam);

/**
 *  @brief Parent function of handler for button4
 *  @details the Parent function of the handler for button4 including debouncing code
 *  @param vpParam - arguments to be passed into the interrupt service routine for button4
 *  @return none
 */
void button4_handler(void* vpParam);

#endif
