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
  * @file UART_debugger_main.c
  * @brief Main source file for testing the UART debugger
  *
  * @details This file contains the source code for the UART debugger driver test application
  *
  * @author Andrew Gawlik
  * @date May 3 2021
  *
  */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "UART_debugger.h"

/**
 * @brief Creates the task used for the UART debugger
 * @details Creates the FreeRTOS task used for the UART debugger
 * @param arg - task arguments currently not used
 * @return none
 *
 */
static void debug_task(void *arg)
{
  debug_init();  //initialize the debugger
  while(1)
  {
    debug_out("Hello World\r\n",13);  //output Hello World a carriage return and a new line to the debugger
  }
}

/**
 * @brief Main application for UART debugger test application
 * @details Creates a task for the UART debugger
 * @return none
 *
 */
void app_main(void)
{  
  xTaskCreate(debug_task, "uart_debugger_task", 1024, NULL, 10, NULL); //use debug_task as the function with a name "uart_debugger_task", a stack size of 1024, no parameters, a priority of 10, and no handle
}
