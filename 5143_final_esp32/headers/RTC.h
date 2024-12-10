/* This file is used to communicate with an external real-time clock device
* Communication is done over I2C.
*/
#ifndef RTC_H
#define RTC_H

#include <RTClib.h>
#include "WifiAndServer.h"
#include "Data.h"
#include "LcdPeltierFans.h"

#define CLOCK_INTERRUPT_PIN 15

// Begins communication, resets clock if power was lost, and sets initial alarms
void setupRTC();

// Set two alarms, one for morning - system on, and one for night - system off
void setAlarms();

// Checks which timer fired and reacts according to day or night
void onTimerExpired();

// Sets timer alert flag when a timer expires
void alarmISR();

#endif