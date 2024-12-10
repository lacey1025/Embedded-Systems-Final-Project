/* This file is used for communcation with the atMega3208.
* Communication is done over UART and all variables are sent and
* received in a single transmission once per second. These variables
* include sensor readings from the atMega, data collected by the ESP32
* from the smartphone application, state changes detected by either
* controller, and instructions from the atMega to turn
* on or off various peripherals. */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Data.h"
#include "WifiAndServer.h"
#include "MusicPlayer.h"

// Send a single byte of data over UART to atMega3208
bool sendByte(uint8_t byte);

// Send information collected from the smartphone application
void sendThresholdData();

// Receive sensor readings from atMega3208
void receiveCurrentData();

// Check if atMega wants to send data, if so, determine what data
// must be sent and received
uint8_t checkForData();

#endif