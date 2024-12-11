/*
 * File:   humidity_control.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "humidity_control.h"

static uint8_t minHumidity;
static uint8_t currentHumidity;
static uint32_t bits;
    

void tsk_humidity_control(void *pvParams) {
    while (1) {
        // Task will sleep until either state that is not NIGHT_MODE is set to 1
        bits = xEventGroupWaitBits(
                stateEventGroup,                 // Event group to wait on
                STATE_ALL_ON | STATE_LIGHTS_OFF, // bits to wait on
                pdFALSE,                         // don't clear bits after waiting
                pdFALSE,                         // don't wait for all bits
                portMAX_DELAY);                  // delay indefinitely
        
        // Safely collect minimum humidity and humidity sensor reading
        if (xSemaphoreTake(threshold_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
            minHumidity = thresholdData.minHumidity;
			xSemaphoreGive(threshold_data_mutex);
		}
        
        if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
			currentHumidity = sensorData.currentHumidity;
			xSemaphoreGive(sensor_data_mutex);
		}
        
        // Check if the water level switch is open
        if (water_level_switch.port->IN & water_level_switch.bm) {
            // update water level state
            xEventGroupSetBits(stateEventGroup, WATER_LEVEL_OKAY);
            // check if watering needed
            if (currentHumidity < minHumidity) {
                humidifier.port->OUT |= humidifier.bm; // turn on humidifier
                
                // update watering mode state
                xEventGroupSetBits(stateEventGroup, STATE_WATERING_MODE);
                
                // This delay is used to set the time for the humidifiers to run.
                // This way ensures that watering is synced to music track2.
                uint8_t count = 0;
                while (count < 135) {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    count++;
                }
                // turn off humidifier
                humidifier.port->OUT &= !humidifier.bm;
                xEventGroupClearBits(stateEventGroup, STATE_WATERING_MODE);
            } 
        } else { // water level low
            xEventGroupClearBits(stateEventGroup, WATER_LEVEL_OKAY);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}






