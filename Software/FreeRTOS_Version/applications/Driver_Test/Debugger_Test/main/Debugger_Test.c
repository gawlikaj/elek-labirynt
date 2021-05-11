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
#include "Helper/StringHelper.h"
#include "UART/Debugger.h"

static uint8_t debugHandle;

static char testOutput[] = "Loop Counter: ##\r\n";  //use # for placeholder to help identify max digits for number to output and position
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
  //uint8_t remainder, digit;
  
  //remainder = loopCount;
  helper_string_insert_uint32(testOutput,19,14,loopCount,2); //insert the loop count into position 14 with a max digit count of 2; the string length of testOutput is 19
  /*
  digit = remainder/10;
  testOutput[14] = (digit>0)?(('0'+digit)):' ';
  digit = remainder%10;
  testOutput[15] = ('0'+digit);
  */
  uart_debugger_out(debugHandle,testOutput,19);  //output the testOutput string
  
  if(loopCount == 99) loopCount = 0;  //reset the loop count when it reaches 99
  else loopCount++;                   //increment the loop count by 1
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
  
  debugTimerHandle = xTimerCreate("DebugTimer",pdMS_TO_TICKS( 500 ),pdTRUE,NULL,vTimerCallback);  //call every 500 milliseconds
  
  
  xTaskCreate(uart_debugger_task, "uart_debugger_task", 1024, NULL, 10, NULL); //use uart_debugger_task as the function with a name "uart_debugger_task", a stack size of 1024, no parameters, a priority of 10, and no handle
  
  loopCount = 0;
  
  xTimerStart(debugTimerHandle,0);
}
