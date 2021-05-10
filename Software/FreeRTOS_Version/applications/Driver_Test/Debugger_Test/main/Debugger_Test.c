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

static char testOutput[19];
static uint8_t loopCount;

/**
 * @brief Timer callback for testing the UART debugger
 * @details outputs "Hello World" to the debugger
 * @param pxTimer the timer handle
 * @return none
 *
 */
void vTimerCallback( TimerHandle_t pxTimer )
{
  uint8_t remainder, digit;
  
  remainder = loopCount;
  digit = remainder/10;
  testOutput[14] = (digit>0)?(('0'+digit)):' ';
  digit = remainder%10;
  testOutput[15] = ('0'+digit);
  uart_debugger_out(debugHandle,testOutput,19);  //output Hello World a carriage return and a new line to the debugger
  
  if(loopCount == 99) loopCount = 0;
  else loopCount++;
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
  
  uart_debugger_init();  //initialize the debugger
  debugHandle = uart_debugger_add_handle();
  
  debugTimerHandle = xTimerCreate("DebugTimer",1000,pdTRUE,NULL,vTimerCallback);
  
  
  xTaskCreate(uart_debugger_task, "uart_debugger_task", 1024, NULL, 10, NULL); //use uart_debugger_task as the function with a name "uart_debugger_task", a stack size of 1024, no parameters, a priority of 10, and no handle
  
  testOutput[0] = 'L';
  testOutput[1] = 'o';
  testOutput[2] = 'o';
  testOutput[3] = 'p';
  testOutput[4] = ' ';
  testOutput[5] = 'C';
  testOutput[6] = 'o';
  testOutput[7] = 'u';
  testOutput[8] = 'n';
  testOutput[9] = 't';
  testOutput[10] = 'e';
  testOutput[11] = 'r';
  testOutput[12] = ':';
  testOutput[13] = ' ';
  testOutput[14] = '#';
  testOutput[15] = '#';
  testOutput[16] = '\r';
  testOutput[17] = '\n';
  testOutput[18] = '\0';
  loopCount = 0;
  
  xTimerStart(debugTimerHandle,0);
}
