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
  * @file QwiicController.c
  * @brief Qwiic Controller Device Driver (source) using the ESP32 as a Controller
  *
  * @details This file contains the source code for the Qwiic Device Driver via I2C as a Controller
  *
  * @author Andrew Gawlik
  * @date May 3 2021
  *
  */

#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "Qwiic/QwiicController.h"

#define I2C_CONTROLLER_RX_BUF_DISABLE  0
#define I2C_CONTROLLER_TX_BUF_DISABLE  0
#define I2C_MODE_CONTROLLER            I2C_MODE_MASTER  //updated terminology from Master to Controller, Slave to Peripheral
#define I2C_CONTROLLER_READ            I2C_MASTER_READ
#define I2C_CONTROLLER_WRITE           I2C_MASTER_WRITE
#define i2c_controller_start           i2c_master_start
#define i2c_controller_write_byte      i2c_master_write_byte
#define i2c_controller_read_byte       i2c_master_read_byte
#define i2c_controller_stop            i2c_master_stop
#define i2c_controller_cmd_begin       i2c_master_cmd_begin

/*
  initialize the qwiic controller driver (uses I2C protocol)
  */
void qwiic_controller_init(void) 
{
  //configure
  i2c_config_t i2c_config = {
    .mode = I2C_MODE_CONTROLLER,  //set ESP32 as controller to access the peripheral Qwiic Joystick
    .sda_io_num = CONFIG_QWIIC_SDA_PIN,  //this is where the qwiic connector is connected for sda
    .scl_io_num = CONFIG_QWIIC_SCL_PIN,  //this is where the qwiic connector is connected for scl
    .sda_pullup_en = true,     
    .scl_pullup_en = true,   
    .master.clk_speed = CONFIG_QWIIC_CONTROLLER_FREQUENCY
    
  };
  
  //install
  i2c_driver_install(CONFIG_QWIIC_PORT_NUM, I2C_MODE_CONTROLLER, I2C_CONTROLLER_RX_BUF_DISABLE, I2C_CONTROLLER_TX_BUF_DISABLE, /*intr_alloc_flags*/0);  //I2C Controller does not need either tx or rx buffers
  
  ESP_ERROR_CHECK(i2c_param_config(CONFIG_QWIIC_PORT_NUM, &i2c_config));
  
  i2c_set_pin(CONFIG_QWIIC_PORT_NUM, CONFIG_QWIIC_SDA_PIN, CONFIG_QWIIC_SCL_PIN, false, false, I2C_MODE_CONTROLLER);
}

esp_err_t qwiic_controller_write_to_peripheral(uint8_t peripheralAddress, uint8_t registerToWrite, uint8_t dataToWrite)
{
  i2c_cmd_handle_t cmdHandle;
  esp_err_t result;
  
  cmdHandle = i2c_cmd_link_create();
  
  i2c_controller_start(cmdHandle);
  i2c_controller_write_byte(cmdHandle,(peripheralAddress << 1) | I2C_CONTROLLER_WRITE,true);
  i2c_controller_write_byte(cmdHandle,registerToWrite,true);
  i2c_controller_write_byte(cmdHandle,dataToWrite,true);
  i2c_controller_stop(cmdHandle);
  
  result = i2c_controller_cmd_begin(CONFIG_QWIIC_PORT_NUM,cmdHandle,1000 / portTICK_RATE_MS);
  ESP_LOGI("Qwiic Write","Address=0x%x  Register=0x%x Data=%d (%s)",peripheralAddress,registerToWrite,dataToWrite,esp_err_to_name(result));
  i2c_cmd_link_delete(cmdHandle);
  
  return result;
}

esp_err_t qwiic_controller_read_from_peripheral(uint8_t peripheralAddress, uint8_t registerToRead, uint8_t *dataRead)
{
  i2c_cmd_handle_t cmdHandleReader,cmdHandleWriter;
  esp_err_t result;
  
  cmdHandleWriter = i2c_cmd_link_create();
  
  i2c_controller_start(cmdHandleWriter);
  i2c_controller_write_byte(cmdHandleWriter,(peripheralAddress << 1) | I2C_CONTROLLER_WRITE,true);
  i2c_controller_write_byte(cmdHandleWriter,registerToRead,true);
  i2c_controller_stop(cmdHandleWriter);
  result = i2c_controller_cmd_begin(CONFIG_QWIIC_PORT_NUM,cmdHandleWriter,1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmdHandleWriter);
  ESP_LOGI("Qwiic Read","Address=0x%x Register=0x%x (%s)",peripheralAddress,registerToRead,esp_err_to_name(result));
  
  cmdHandleReader = i2c_cmd_link_create();
  i2c_controller_start(cmdHandleReader);
  i2c_controller_write_byte(cmdHandleReader,(peripheralAddress << 1)  | I2C_CONTROLLER_READ,true);
  i2c_controller_read_byte(cmdHandleReader,dataRead,true);
  i2c_controller_stop(cmdHandleReader);
  result = i2c_controller_cmd_begin(CONFIG_QWIIC_PORT_NUM,cmdHandleReader,1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmdHandleReader);
  
  ESP_LOGI("Qwiic Read","Address=0x%x Data=%d (%s)",peripheralAddress,*dataRead,esp_err_to_name(result));
  
  return result;
}


int qwiic_controller_get_timeout()
{
  int timeout;
  i2c_get_timeout(CONFIG_QWIIC_PORT_NUM,&timeout);
  return timeout;
}


void qwiic_controller_set_timeout(int timeout)
{
  i2c_set_timeout(CONFIG_QWIIC_PORT_NUM,timeout);
}

uint8_t qwiic_controller_is_peripheral_connected(uint8_t peripheralAddress)
{
  i2c_cmd_handle_t cmdHandle;
  esp_err_t result;
  
  cmdHandle = i2c_cmd_link_create();
  i2c_controller_start(cmdHandle);
  i2c_controller_write_byte(cmdHandle,(peripheralAddress << 1) | I2C_CONTROLLER_WRITE,true);
  i2c_controller_stop(cmdHandle);
  result = i2c_controller_cmd_begin(CONFIG_QWIIC_PORT_NUM,cmdHandle,1000/portTICK_RATE_MS);
  i2c_cmd_link_delete(cmdHandle);
  
  return (result == ESP_OK);
}
