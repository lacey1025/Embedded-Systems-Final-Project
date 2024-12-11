/* 
 * File:   esp32-uart.h
 * Author: laceysemansky
 *
 * Created on November 16, 2024, 4:21 PM
 */

#ifndef ESP32_UART_H
#define	ESP32_UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "data.h"
#include "FreeRTOS.h"
#include "task.h"

void initUART();
void receiveThresholdData(ThresholdData* data);
void receiveDayOrNight(uint8_t* buffer);
void sendCurrentData(SensorData* data, ThresholdData* thresholds, uint8_t* buf);

#endif	/* ESP32_UART_H */

