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
  * @file buttons.c
  * @brief Momentary Pushbuttons Interface (source)
  *
  * @details This file contains the source code for the Momentary Pushbuttons using Free RTOS on the ESP32
  *
  * @author Andrew Gawlik
  * @date Jul 1 2021
  *
  */

#include "buttons.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

static const TickType_t ticksDeltaDebounce = pdMS_TO_TICKS(CONFIG_BUTTONS_DEBOUNCE_MS);                          //convert the debounce time configured in menuconfig to number of ticks

#define BUTTONS_NUM  5                                                                                           //the number of buttons
static const gpio_num_t buttonPin[BUTTONS_NUM] = {GPIO_NUM_32,GPIO_NUM_33,GPIO_NUM_12,GPIO_NUM_13,GPIO_NUM_36};  //array for each number
ButtonHandler handlers[BUTTONS_NUM];
static const gpio_isr_t parentHandlers[BUTTONS_NUM] = {button0_handler,button1_handler,button2_handler,button3_handler,button4_handler};

esp_err_t buttons_init(void)
{
  uint8_t indexButton;
  gpio_num_t currentGpio;
  
  for(indexButton = 0; indexButton<BUTTONS_NUM; indexButton++)//iterate through the buttonPin array of GPIO pins
  {
    currentGpio = buttonPin[indexButton];                     //current gpio pin being configured
    gpio_reset_pin(currentGpio);                              //resets the pin configuration for GPIO function via IO_MUX
    
    gpio_set_direction(currentGpio,GPIO_MODE_INPUT);          //configure GPIO pin assigned to the button index as input
    gpio_sleep_set_direction(currentGpio,GPIO_MODE_INPUT);    //GPIO is still input only
    
    gpio_set_pull_mode(currentGpio,GPIO_FLOATING);            //GPIO is not using internal resistors (only external pull-down)
    gpio_sleep_set_pull_mode(currentGpio,GPIO_FLOATING);      //GPIO is not using internal resistors
    gpio_wakeup_disable(currentGpio);                         //wakeup only works with high or low levels
    
    gpio_set_intr_type(currentGpio,GPIO_INTR_POSEDGE);        //interrupt on the positive edge of input signal
    gpio_intr_disable(currentGpio);                           //keep interrupt disabled on pin until an interrupt service routine is assigned
    
    if(GPIO_IS_VALID_OUTPUT_GPIO(currentGpio))                //check to see which GPIO pins are valid output pins (to disable output functionality)
    {
      gpio_set_drive_capability(currentGpio,GPIO_DRIVE_CAP_0);//set drive strength 0 (because it's an input not an output)
    }
    
    //gpio_wakeup_enable(currentGpio,GPIO_INTR_HIGH_LEVEL);     //only wake on logic 1 (high level) signal
    gpio_sleep_sel_en(currentGpio);                           //allow switching between light sleep and normal for ISR
  }
  
  return gpio_install_isr_service(0);                         //install the GPIO ISR service for per-pin interrupt handlers
}

uint8_t buttons_read(ButtonId selButtonId)
{
  return gpio_get_level(buttonPin[selButtonId]);                 //retrieve the logic level of the button
}

esp_err_t buttons_add_handler(ButtonId selButtonId, ButtonHandler buttonHandler, void* args)
{
  handlers[selButtonId] = buttonHandler;
  return gpio_isr_handler_add(buttonPin[selButtonId],parentHandlers[selButtonId],args); //register the handler to the gpio interrupt service routine
}

esp_err_t buttons_remove_handler(ButtonId selButtonId)
{
  esp_err_t ret;
  ret = gpio_isr_handler_remove(buttonPin[selButtonId]);
  handlers[selButtonId] = NULL;
  return ret;
}

void buttons_deinit(void)
{
  gpio_uninstall_isr_service();
}

IRAM_ATTR void button0_handler(void* vpParam)
{
  static TickType_t ticksLastPressed = 0;                     //last press ticks saved from last activation
  TickType_t currentTicks = xTaskGetTickCountFromISR();       //current ticks from this call

  if((currentTicks - ticksLastPressed) > ticksDeltaDebounce)  //if the number of ticks between the current call and last activation is greater than the debounce ticks
  {                                                           //proceed normally
    handlers[BUTTON0](vpParam);                               //call user-assigned handler with user-assigned argument
    ticksLastPressed = currentTicks;                          //set last press ticks
  }                                                           //when the difference in ticks is within the debounce ticks ignore
}

IRAM_ATTR void button1_handler(void* vpParam)
{
  static TickType_t ticksLastPressed = 0;                     //last press ticks saved from last activation
  TickType_t currentTicks = xTaskGetTickCountFromISR();       //current ticks from this call

  if((currentTicks - ticksLastPressed) > ticksDeltaDebounce)  //if the number of ticks between the current call and last activation is greater than the debounce ticks
  {                                                           //proceed normally
    handlers[BUTTON1](vpParam);                               //call user-assigned handler with user-assigned argument
    ticksLastPressed = currentTicks;                          //set last press ticks
  }                                                           //when the difference in ticks is within the debounce ticks ignore
}

IRAM_ATTR void button2_handler(void* vpParam)
{
  static TickType_t ticksLastPressed = 0;                     //last press ticks saved from last activation
  TickType_t currentTicks = xTaskGetTickCountFromISR();       //current ticks from this call

  if((currentTicks - ticksLastPressed) > ticksDeltaDebounce)  //if the number of ticks between the current call and last activation is greater than the debounce ticks
  {                                                           //proceed normally
    handlers[BUTTON2](vpParam);                               //call user-assigned handler with user-assigned argument
    ticksLastPressed = currentTicks;                          //set last press ticks
  }                                                           //when the difference in ticks is within the debounce ticks ignore
}

IRAM_ATTR void button3_handler(void* vpParam)
{
  static TickType_t ticksLastPressed = 0;                     //last press ticks saved from last activation
  TickType_t currentTicks = xTaskGetTickCountFromISR();       //current ticks from this call

  if((currentTicks - ticksLastPressed) > ticksDeltaDebounce)  //if the number of ticks between the current call and last activation is greater than the debounce ticks
  {                                                           //proceed normally
    handlers[BUTTON3](vpParam);                               //call user-assigned handler with user-assigned argument
    ticksLastPressed = currentTicks;                          //set last press ticks
  }                                                           //when the difference in ticks is within the debounce ticks ignore
}

IRAM_ATTR void button4_handler(void* vpParam)
{
  static TickType_t ticksLastPressed = 0;                     //last press ticks saved from last activation
  TickType_t currentTicks = xTaskGetTickCountFromISR();       //current ticks from this call

  if((currentTicks - ticksLastPressed) > ticksDeltaDebounce)  //if the number of ticks between the current call and last activation is greater than the debounce ticks
  {                                                           //proceed normally
    handlers[BUTTON4](vpParam);                               //call user-assigned handler with user-assigned argument
    ticksLastPressed = currentTicks;                          //set last press ticks
  }                                                           //when the difference in ticks is within the debounce ticks ignore
}
