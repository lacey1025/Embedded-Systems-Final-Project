/*
 * File:   data.c
 * Author: laceysemansky
 *
 * Created on November 1, 2024, 8:35 PM
 */

#define F_CPU 3333333

#include "data.h"

SensorData sensorData = {
    .currentLight = 1200,
    .currentTemperature = 50,
    .currentHumidity = 20
};

ThresholdData thresholdData = {
    .light = 1000,
    .maxTemperature = 30,
    .minTemperature = 15,
    .maxHumidity = 70,
    .minHumidity = 20
};

void setStartupEventBits() {
    xEventGroupClearBits(stateEventGroup, STATE_LIGHTS_OFF | STATE_NIGHT_MODE | 
                                          STATE_WATERING_MODE | STATE_COOLING_MODE | FAN_ON);
    xEventGroupSetBits(stateEventGroup, STATE_ALL_ON | MUSIC_ON | WATER_LEVEL_OKAY | MUSIC_PREVIOUSLY_ON);
}