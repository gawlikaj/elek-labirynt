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
 * @file debugger.c
 * @brief UART Debugger Driver (source)
 *
 * @details This file contains the source code for the UART debugger driver
 *
 * @author Andrew Gawlik
 * @date May 3 2021
 *
 */

#include "UART/Debugger.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/message_buffer.h"


#define UART_DEBUGGER_TXD  (GPIO_NUM_1)
#define UART_DEBUGGER_RXD  (GPIO_NUM_3)
#define UART_DEBUGGER_RTS  (GPIO_NUM_2)
#define UART_DEBUGGER_CTS  (GPIO_NUM_35)

#define DEBUG_MAX_HANDLES 16  //no more than 16 debug handles allowed
#define BUF_SIZE (128)

#define UART_DEBUGGER_PORT UART_NUM_0  //UART0 being used for the debug port since it is connected directly to the USB port

uint8_t countDebugHandles;
MessageBufferHandle_t arrDebugHandles[DEBUG_MAX_HANDLES];

static char msgErrHandle[33] = "Handle ## exceeded buffer size\r\n";


/*
    initialize the UART for the debugger
 */
void uart_debugger_init()
{
  /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,                   //115200 bits/second
        .data_bits = UART_DATA_8_BITS,         //use 8 data bits
        .parity    = UART_PARITY_DISABLE,      //no parity
        .stop_bits = UART_STOP_BITS_1,         //1 stop bit
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, //turn off flow control
        .source_clk = UART_SCLK_APB,           //80MHz APB clock
    };
    
    
    /*
       install the driver
       
       using the defined UART_DEBUGGER_PORT to specify the UART to use
       receive buffer size is BUF_SIZE*2
       transmit buffer size is 0  (not using transmit buffer)
       UART event queue size is 0
       not using Queue Handle for UART driver (does not handle UART events)
       intr_alloc_flags not set  //use esp_intr_alloc.h for possible flags, do not use ESP_INTR_FLAG_IRAM (as stated in Line 94 of uart.h)
     */       
    uart_driver_install(UART_DEBUGGER_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);
    
    /*
       configure UART parameters being set for the UART_DEBUGGER_PORT and check for errors using ESP_ERROR_CHECK
     */
    ESP_ERROR_CHECK(uart_param_config(UART_DEBUGGER_PORT, &uart_config));
    
    /*
       set the pins for the UART_DEBUGGER_PORT
       Transmit Pin
       Receive Pin
       RTS pin
       CTS pin
     */
    uart_set_pin(UART_DEBUGGER_PORT, UART_DEBUGGER_TXD, UART_DEBUGGER_RXD, UART_DEBUGGER_RTS, UART_DEBUGGER_CTS);
    
    countDebugHandles = 0;
}

/*
  the task used for the debugger
 */
void uart_debugger_task()
{
  int debuggerHandleIndex;
  char buffer[BUF_SIZE];
  static const TickType_t xBlockTime = pdMS_TO_TICKS( 20 );
  size_t receivedBytes;
  uint8_t remainder, digit;
  
 
  while(1)
  {
    for(debuggerHandleIndex = 0; debuggerHandleIndex < countDebugHandles; debuggerHandleIndex++)
   {
     receivedBytes = xMessageBufferReceive(arrDebugHandles[debuggerHandleIndex],buffer,BUF_SIZE,xBlockTime);
     if(receivedBytes > 0)
     {
       if(receivedBytes < BUF_SIZE)
	  uart_write_bytes(UART_DEBUGGER_PORT,(const char*)buffer,receivedBytes);
       else
       {
               
          remainder = debuggerHandleIndex;
          digit = remainder/10;
          msgErrHandle[8] = (digit>0)?(('0'+digit)):' ';
          digit = remainder%10;
          msgErrHandle[9] = ('0'+digit);
  
          uart_write_bytes(UART_DEBUGGER_PORT,msgErrHandle,33);
       }
     }
   }
  }
}

/*
  generates a debug message handle
*/
uint8_t uart_debugger_add_handle()
{
  MessageBufferHandle_t handle;
  uint8_t result;  //assigned index
  
  if(countDebugHandles < DEBUG_MAX_HANDLES)  //ensure that we are within the limits of the debug handles
  {
    handle = xMessageBufferCreate(BUF_SIZE);  //may still result in null if the heap cannot allocate it, but attempt to allocate in countDebugHandles
  }
  else  //more than allowed message buffers to be created
  {
    handle = NULL;
  }
  
  if(handle != NULL)
  {
    result = countDebugHandles;
    arrDebugHandles[result] = handle; //assign message buffer to array
    countDebugHandles++;                         //increment the count of debug handles
  }
  else
  {
    result = DEBUG_MAX_HANDLES;
  }
  
  return result;
}

/*
    output a string via the UART debugger
 */
void uart_debugger_out(uint8_t handleIndex, char* data, size_t len)
{
  const TickType_t xBlockTime = pdMS_TO_TICKS( 20 );    
  
  if(handleIndex < countDebugHandles)
  {
    xMessageBufferSend(arrDebugHandles[handleIndex], (void *) data, len, xBlockTime); //write a string (data) of length len to the UART for debugging
  }
}
