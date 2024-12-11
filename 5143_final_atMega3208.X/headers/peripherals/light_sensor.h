/* 
 * File:   light_sensor.h
 * Author: laceysemansky
 *
 * Created on October 19, 2024, 8:39 AM
 */

#ifndef LIGHT_SENSOR_H
#define	LIGHT_SENSOR_H

#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"

#define LIGHT_SENSOR_ADDR 0x29
#define READ (0x01)
#define WRITE (0x00)
#define COMMAND_BIT (0x80)
#define ENABLE_POWERON (0x01)
#define ENABLE_POWEROFF (0x00)
#define GAIN_LOW (0x00)
#define GAIN_MED (0x01)  /// medium gain (25x)
#define INTEGRATIONTIME_100MS (0x00)
#define REGISTER_ENABLE (0x00)
#define REGISTER_CONTROL (0x01)
#define REGISTER_CHAN0_LOW (0x14)
#define REGISTER_CHAN0_HIGH (0x15)
#define REGISTER_CHAN1_LOW (0x16)
#define REGISTER_CHAN1_HIGH (0x17)
#define COMMAND_NORMAL_OPERATION (0x20)
#define COMMAND_SPECIAL_OPERATION (0x60)
#define BLOCK_BIT (0x10)
#define ENABLE_AEN (0x02)
#define LUX_DF (408.0F) // Lux coefficient

void lightSensorSetup();
void lightSensorDisable();
uint16_t getLightSensorReading();

// Gain and integration determine how much boosting is needed for light data collected
// and how long the lense collects light information
void setGainAndIntegration();

// Collects raw sensor data
uint32_t getFullLuminosity();

// Calculates the lux value from the raw data
uint16_t calculateLux(uint32_t);

#endif	/* LIGHT_SENSOR_H */

