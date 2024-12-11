/*
 * File:   read_sensors.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "read_sensors.h"

static uint16_t tempAndHum[2];
static uint16_t lux;
    
void tsk_read_sensors(void *pvParams) {
    while (1) {
        // Communicate with SHT-30 temp and humidity sensor
        getTempAndHumidity(tempAndHum);
        
        // Communicate with light sensor
        lux = getLightSensorReading();
        
        // Safely update sensor data
        if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
			sensorData.currentHumidity = tempAndHum[1];
            sensorData.currentTemperature = tempAndHum[0];
            sensorData.currentLight = lux;
			xSemaphoreGive(sensor_data_mutex);
		}
        
        // Holds semaphore to block ESP32 communication while new data is being read.
        // This also ensures that communication only happens once a second.
        // Semaphore is given only after all data is read
        xSemaphoreGive(communication_semaphore);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



