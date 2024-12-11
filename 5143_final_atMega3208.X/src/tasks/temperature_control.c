/*
 * File:   temperature_control.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "temperature_control.h"

static uint8_t maxTemperature;
static uint8_t maxHumidity;
static uint8_t currentHumidity;
static uint8_t currentTemperature;
static uint32_t bits;
    

void tsk_temperature_control(void *pvParams) {
    while (1) {
        bits = xEventGroupGetBits(stateEventGroup);
        
        // Turn off fan and cooling if night mode is entered
        if (bits & STATE_NIGHT_MODE) {
            if (bits & STATE_COOLING_MODE) {
                xEventGroupClearBits(stateEventGroup, STATE_COOLING_MODE);
            }
            if (bits & FAN_ON) {
                xEventGroupClearBits(stateEventGroup, FAN_ON);
            }
            
            // Task will block until state is no longer in night mode
            bits = xEventGroupWaitBits(
                    stateEventGroup,                 // Event group to wait on
                    STATE_ALL_ON | STATE_LIGHTS_OFF, // bits to wait on
                    pdFALSE,                         // don't clear bits after waiting
                    pdFALSE,                         // don't wait for all bits
                    portMAX_DELAY);                  // delay indefinitely
        } else { // not night mode
            // Gather information
            if (xSemaphoreTake(threshold_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                maxTemperature = thresholdData.maxTemperature;
                maxHumidity = thresholdData.maxHumidity;
                xSemaphoreGive(threshold_data_mutex);
            }
            if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                currentTemperature = sensorData.currentTemperature;
                currentHumidity = sensorData.currentHumidity;
                xSemaphoreGive(sensor_data_mutex);
            }

            // Check if temperature is too high
            if (currentTemperature > maxTemperature) {
                xEventGroupSetBits(stateEventGroup, STATE_COOLING_MODE);
            } else if (currentTemperature + 2 < maxTemperature) {
                xEventGroupClearBits(stateEventGroup, STATE_COOLING_MODE);
            }

            // Check if humidity is too high
            if (currentHumidity > maxHumidity) {
                xEventGroupSetBits(stateEventGroup, FAN_ON);
            } else if (currentHumidity + 2 < maxHumidity) {
                xEventGroupClearBits(stateEventGroup, FAN_ON);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}






