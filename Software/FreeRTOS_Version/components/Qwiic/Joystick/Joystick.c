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
  * @file Joystick.c
  * @brief Qwiic I2C Joystick Driver (source)
  *
  * @details This file contains the source code for the Qwiic Joystick Driver using I2C
  *
  * @author Andrew Gawlik
  * @date May 3 2021
  *
  */

#include "esp_log.h"
#include "Qwiic/Joystick.h"
#include "Qwiic/QwiicController.h"

#define QWIIC_JOYSTICK_REG_DEFAULT_ADDR   0x00
#define QWIIC_JOYSTICK_REG_FIRMWARE_MSB   0x01
#define QWIIC_JOYSTICK_REG_FIRMWARE_LSB   0x02
#define QWIIC_JOYSTICK_REG_HORZ_MSB       0x03
#define QWIIC_JOYSTICK_REG_HORZ_LSB       0x04
#define QWIIC_JOYSTICK_REG_VERT_MSB       0x05
#define QWIIC_JOYSTICK_REG_VERT_LSB       0x06
#define QWIIC_JOYSTICK_REG_CURR_SEL       0x07
#define QWIIC_JOYSTICK_REG_STAT_SEL       0x08
#define QWIIC_JOYSTICK_REG_CONFIG_LOCK    0x09
#define QWIIC_JOYSTICK_REG_CURRENT_ADDR   0x0A


/*
   initialize joystick
 */
void qwiic_joystick_init(void)
{
  qwiic_controller_write_to_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_STAT_SEL,0x00); //write 0 back to the status register to clear
}

/*
    read the horizontal position of the joystick
 */
esp_err_t qwiic_joystick_get_horz(uint16_t *xValue)
{
  uint8_t xMSB, xLSB;
  esp_err_t result;
  result = qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_HORZ_MSB,&xMSB); //get the result from the first read
  qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_HORZ_LSB,&xLSB);

  ESP_LOGW("X Axis","%d %d",xMSB,xLSB);
  *xValue = ((uint16_t)xMSB)<<8 | ((uint16_t)xLSB >>6);
//combine MSB and LSB into a single number and return

  return result;
}

/*
   read the vertical position of the joystick
 */
esp_err_t qwiic_joystick_get_vert(uint16_t *yValue)
{
  uint8_t yMSB, yLSB;
  esp_err_t result;
  
  result = qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_VERT_MSB,&yMSB);
  qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_VERT_LSB,&yLSB);

  ESP_LOGW("Y Axis","%d %d",yMSB,yLSB);
  
  *yValue = ((uint16_t)yMSB)<<8 | ((uint16_t)yLSB >>6);
//combine MSB and LSB into a single number and return

  return result;
}

/*
  get the current state of the select button
*/
esp_err_t qwiic_joystick_get_select(uint8_t *selCurrent)
{
  esp_err_t result;
  
  result = qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_CURR_SEL,selCurrent);
  ESP_LOGW("Select","Current=%d",*selCurrent);
  
  return result;
}

/*
   check the status of the select button of the joystick since last read
 */
esp_err_t qwiic_joystick_chk_select(uint8_t *selStatus)
{
  esp_err_t result;
  
  result = qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_STAT_SEL,selStatus);
  ESP_LOGW("Select","Status=%d",*selStatus);
  
  qwiic_controller_write_to_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_STAT_SEL,0x00); //write 0 back to the status register to clear
  
  return result;
}

/*
    read all data from the joystick and store into a struct
 */
void qwiic_joystick_read(QwiicJoystickType* value)
{
  qwiic_joystick_get_horz(&(value->horz));
  qwiic_joystick_get_vert(&(value->vert));
  qwiic_joystick_chk_select(&(value->select));
}

/*
  get firmware version of the joystick
 */
 void qwiic_joystick_version(uint8_t *major, uint8_t *minor)
 {
   qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_FIRMWARE_MSB,major);  //read the major version of the firmware
   qwiic_controller_read_from_peripheral(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS,QWIIC_JOYSTICK_REG_FIRMWARE_LSB,minor);  //read the minor version of the firmware

 }
 
 uint8_t qwiic_joystick_isconnected()
 {
   return qwiic_controller_is_peripheral_connected(CONFIG_QWIIC_JOYSTICK_PERIPHERAL_ADDRESS);
 }
