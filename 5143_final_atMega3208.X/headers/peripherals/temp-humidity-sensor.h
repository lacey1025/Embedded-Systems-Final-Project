/* 
 * File:   temp-humidity-sensor.h
 * Author: laceysemansky
 *
 * Created on November 9, 2024, 9:26 PM
 */

#ifndef TEMP_HUMIDITY_SENSOR_H
#define	TEMP_HUMIDITY_SENSOR_H

#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"

void getRawData(uint32_t *buf);
void getTempAndHumidity(uint16_t *buf);
uint16_t getTemperature();
uint16_t getHumidityPercentage();


#endif	/* TEMP_HUMIDITY_SENSOR_H */

