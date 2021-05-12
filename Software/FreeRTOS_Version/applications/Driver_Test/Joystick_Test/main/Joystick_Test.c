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
  * @file Joystick_Test.c
  * @brief Main source file for testing the Qwiic Joystick
  *
  * @details This file contains the source code for the Qwiic Joystick driver test application
  *
  * @author Andrew Gawlik
  * @date May 3 2021
  *
  */
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "Qwiic/QwiicController.h"
#include "Qwiic/Joystick.h"

#define ERR_MSG_LEN 21

static const char statusText[3][8] = {"    OFF","     ON","UNKNOWN"};
static uint8_t sel_btn;
static uint16_t horz;
static uint16_t vert;

void outputErrorMessage(const char* param, esp_err_t errCode)
{
  switch(errCode)
  {
    case ESP_OK:
    {
      ESP_LOGW("Joystick", "%s - Retrieve Success", param);
    }
    break;
    case ESP_FAIL:
    {
      ESP_LOGE("Joystick", "%s - No Ack from Joystick", param);
    }
    break;
    case ESP_ERR_INVALID_STATE:
    {
      ESP_LOGE("Joystick", "%s - Invalid State (Check for Driver Installed or Controller Mode)", param);
    }
    break;
    case ESP_ERR_TIMEOUT:
    {
      ESP_LOGE("Joystick", "%s - Retrieve Timeout", param);
    }
    break;
    default:
    {
      ESP_LOGE("Joystick", "%s - Unknown error", param);
    }
  }
}


/**
 * @brief Timer callback for testing the UART debugger
 * @details outputs "Hello World" to the debugger
 * @param pxTimer the timer handle
 * @return none
 *
 */
void vTimerCallback( TimerHandle_t pxTimer )
{
  esp_err_t errSel, errX, errY;
  
  errSel = qwiic_joystick_chk_select(&sel_btn);
  errX = qwiic_joystick_get_horz(&horz);
  errY = qwiic_joystick_get_vert(&vert);
  
  if(sel_btn > 1) sel_btn = 2;
  
  outputErrorMessage("Select",errSel);
  outputErrorMessage("Horizontal",errX);
  outputErrorMessage("Vertical",errY);  
  
  ESP_LOGI("Joystick","x = %4d, y = %4d, SELECT = %s",horz,vert,statusText[sel_btn]);  //output joyDebug string consisting of x-axis, y-axis and select button readouts
}

/**
 * @brief Main application for UART debugger test application
 * @details Creates a task for the UART debugger
 * @return none
 *
 */
void app_main(void)
{
  TimerHandle_t joystickTimerHandle; //handle for a debug timer
  uint8_t qwiicJoystickMajorVersion, qwiicJoystickMinorVersion;
  uart_set_pin(UART_NUM_0, GPIO_NUM_1, GPIO_NUM_3, GPIO_NUM_2, GPIO_NUM_35);
  qwiic_controller_init();
  qwiic_joystick_init();
  ESP_LOGI("Qwiic Joystick","Connected = %d",qwiic_joystick_isconnected());
  ESP_LOGI("Qwiic Timeout","%d",qwiic_controller_get_timeout());
  
  qwiic_joystick_version(&qwiicJoystickMajorVersion,&qwiicJoystickMinorVersion);
  ESP_LOGI("Qwiic Joystick","v%d.%d",qwiicJoystickMajorVersion,qwiicJoystickMinorVersion);  
  joystickTimerHandle = xTimerCreate("JoystickTimer",pdMS_TO_TICKS( 1000 ),pdTRUE,NULL,vTimerCallback);

  xTimerStart(joystickTimerHandle,0);
}
