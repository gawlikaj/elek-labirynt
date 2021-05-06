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
  * @file Debugger_Test.c
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
#include "freertos/timers.h"
#include "UART/Debugger.h"

static uint8_t debugHandle;

/**
 * @brief Timer callback for testing the UART debugger
 * @details outputs "Hello World" to the debugger
 * @param pxTimer the timer handle
 * @return none
 *
 */
void vTimerCallback( TimerHandle_t pxTimer )
{
  debug_out(debugHandle,"Hello World\r\n",13);  //output Hello World a carriage return and a new line to the debugger
}

/**
 * @brief Main application for UART debugger test application
 * @details Creates a task for the UART debugger
 * @return none
 *
 */
void app_main()
{  
  TimerHandle_t debugTimerHandle; //handle for a debug timer
  
  debug_init();  //initialize the debugger
  debugHandle = debug_add_handle();
  
  debugTimerHandle = xTimerCreate("DebugTimer",1000,pdTRUE,NULL,vTimerCallback);
  
  
  xTaskCreate(debug_task, "uart_debugger_task", 1024, NULL, 10, NULL); //use debug_task as the function with a name "uart_debugger_task", a stack size of 1024, no parameters, a priority of 10, and no handle
  
  xTimerStart(debugTimerHandle,0);
}
