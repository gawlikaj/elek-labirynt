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


#define UART_DEBUG_TXD  (GPIO_NUM_1)
#define UART_DEBUG_RXD  (GPIO_NUM_3)
#define UART_DEBUG_RTS  (GPIO_NUM_2)
#define UART_DEBUG_CTS  (GPIO_NUM_35)

#define BUF_SIZE (1024)

#define UART_DEBUG_PORT UART_NUM_0  //UART0 being used for the debug port since it is connected directly to the USB port


/*
    initialize the UART for the debugger
 */
void debug_init(void)
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
       
       using the defined UART_DEBUG_PORT to specify the UART to use
       receive buffer size is BUF_SIZE*2
       transmit buffer size is 0  (not using transmit buffer)
       UART event queue size is 0
       not using Queue Handle for UART driver (does not handle UART events)
       intr_alloc_flags not set  //use esp_intr_alloc.h for possible flags, do not use ESP_INTR_FLAG_IRAM (as stated in Line 94 of uart.h)
     */       
    uart_driver_install(UART_DEBUG_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);
    
    /*
       configure UART parameters being set for the UART_DEBUG_PORT and check for errors using ESP_ERROR_CHECK
     */
    ESP_ERROR_CHECK(uart_param_config(UART_DEBUG_PORT, &uart_config));
    
    /*
       set the pins for the UART_DEBUG_PORT
       Transmit Pin
       Receive Pin
       RTS pin
       CTS pin
     */
    uart_set_pin(UART_DEBUG_PORT, UART_DEBUG_TXD, UART_DEBUG_RXD, UART_DEBUG_RTS, UART_DEBUG_CTS);
}


/*
    output a string via the UART debugger
 */
void debug_out(char* data, size_t len)
{
  uart_write_bytes(UART_DEBUG_PORT, (const char *) data, len); //write a string (data) of length len to the UART for debugging
}
