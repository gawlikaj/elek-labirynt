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
 * @file Buttons_Test.c
 * @brief Momentary Pushbuttons Interactive Test
 *
 * @details This file contains an interactive test application for the Momentary Pushbuttons using FreeRTOS on the ESP32
 *
 * @author Andrew Gawlik
 * @date Jul 7 2021
 *
 */

#include <stdio.h>
#include "buttons.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

const char *TAG = "Button";
#define NUMBER_OF_BUTTONS 5

uint16_t pressButtonRed,pressButtonYellow,pressButtonBlue,pressButtonGreen,pressButtonBlack;

void press_red()
{
  pressButtonRed++;
}

void press_yellow()
{
  pressButtonYellow++;
}

void press_green()
{
  pressButtonGreen++;
}

void press_blue()
{
  pressButtonBlue++;
}

void press_black()
{
  pressButtonBlack++;
}

const char *statusButton[2] = {"OFF"," ON"};

void count_presses(TimerHandle_t pxTimer)
{
  uint8_t button[NUMBER_OF_BUTTONS];
  uint8_t buttonIndex;
  
  for(buttonIndex = 0; buttonIndex < NUMBER_OF_BUTTONS; buttonIndex++)
  {
    button[buttonIndex] = buttons_read(buttonIndex);
  }

  ESP_LOGI(TAG,"Button0=%d, Button1=%d, Button2=%d, Button3=%d, Button4=%d",pressButtonRed,pressButtonYellow,pressButtonGreen,pressButtonBlue,pressButtonBlack);
  ESP_LOGI(TAG,"Button0=%s, Button1=%s, Button2=%s, Button3=%s, Button4=%s",statusButton[button[0]],statusButton[button[1]],statusButton[button[2]],statusButton[button[3]],statusButton[button[4]]);
}

void app_main(void)
{
  TimerHandle_t buttonTimerHandle; //handle for a debug timer
  buttons_init();
  
  pressButtonRed = 0;
  pressButtonYellow = 0;
  pressButtonGreen = 0;
  pressButtonBlue = 0;
  pressButtonBlack = 0;
  
  buttons_add_handler(0,press_red,NULL);
  buttons_add_handler(1,press_yellow,NULL);
  buttons_add_handler(2,press_green,NULL);
  buttons_add_handler(3,press_blue,NULL);
  buttons_add_handler(4,press_black,NULL);
  
  buttonTimerHandle = xTimerCreate("ButtonsTimer",pdMS_TO_TICKS( 1000 ),pdTRUE,NULL,count_presses);

  xTimerStart(buttonTimerHandle,0);
}
