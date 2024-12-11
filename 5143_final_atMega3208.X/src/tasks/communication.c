/*
 * File:   communication.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "communication.h"
    
static ThresholdData localThresholds;
static SensorData localSensorReadings;
static uint8_t buf[4] = { 0, 0, 0, 0 };
static uint32_t bits;


void tsk_communication(void *pvParams) {
    while (1) {
        // Holds semaphore to block ESP32 communication while new data is being read.
        // This also ensures that communication only happens once a second.
        if (xSemaphoreTake(communication_semaphore, portMAX_DELAY) == pdTRUE) {
            // Safely collect sensor readings and current threshold information
            if (xSemaphoreTake(threshold_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                localThresholds = thresholdData;
                xSemaphoreGive(threshold_data_mutex);
            }
            if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                localSensorReadings = sensorData;
                xSemaphoreGive(sensor_data_mutex);
            }
            
            bits = xEventGroupGetBits(stateEventGroup);
            
            // Fill the buffer to send information to the ESP32
            // [0] is the state information
            if (bits & STATE_ALL_ON) {
                buf[0] = 0;
            } else if (bits & STATE_LIGHTS_OFF) {
                buf[0] = 1;
            } else if (bits & STATE_NIGHT_MODE) {
                buf[0] = 2;
            }
            
            // [1] is cooling mode
            if (bits & STATE_COOLING_MODE) {
                buf[1] = 1;
            } else {
                buf[1] = 0;
            }
            // [2] is fan on
            if (bits & FAN_ON) {
                buf[2] = 1;
            } else {
                buf[2] = 0;
            }
            // [3] is music on, track 1, or track 2
            if (bits & MUSIC_ON) { 
                if (bits & STATE_WATERING_MODE) { 
                    buf[3] = 2; 
                } else { 
                    buf[3] = 1; 
                } 
            } else { 
                buf[3] = 0; 
            }
            
            // This sends the updated data, it also receives some information from the ESP32
            sendCurrentData(&localSensorReadings, &localThresholds, buf);
            
            if (xSemaphoreTake(threshold_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                thresholdData = localThresholds;
                xSemaphoreGive(threshold_data_mutex);
            }
            if (xSemaphoreTake(state_mutex, portMAX_DELAY) == pdTRUE) {
                bits = xEventGroupGetBits(stateEventGroup);
                // buf[0] is updated by the ESP32 if the RTC alarm fired for night or day switching
                // here if buf[0] is 2, the day alarm was fired and the system must switch to day mode (all_on)
                if ((bits & STATE_NIGHT_MODE) && buf[0] == 2) {
                    xEventGroupClearBits(stateEventGroup, STATE_NIGHT_MODE | STATE_LIGHTS_OFF);
                    xEventGroupSetBits(stateEventGroup, STATE_ALL_ON);
                    // Only turn on the music if it was previously on
                    if (bits & MUSIC_PREVIOUSLY_ON) {
                        xEventGroupSetBits(stateEventGroup, MUSIC_ON);
                    }
                // if buf[0] is 1, the alarm was fired for night mode and the system must switch to state_night_mode
                } else if (buf[0] == 1) {
                    xEventGroupClearBits(stateEventGroup, STATE_ALL_ON | STATE_LIGHTS_OFF | MUSIC_ON);
                    xEventGroupSetBits(stateEventGroup, STATE_NIGHT_MODE);
                }
                xSemaphoreGive(state_mutex);
            }
        }
    }
}
