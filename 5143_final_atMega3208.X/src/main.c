/*
 * File:   main.c
 * Author: laceysemansky
 *
 * Created on November 1, 2024, 8:35 PM
 */

#define F_CPU 3333333
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "peripherals.h"
#include "data.h"
#include "read_sensors.h"
#include "humidity_control.h"
#include "light_control.h"
#include "music_control.h"
#include "temperature_control.h"
#include "button_control.h"
#include "communication.h"


SemaphoreHandle_t     sensor_data_mutex;
SemaphoreHandle_t     threshold_data_mutex;
SemaphoreHandle_t     state_mutex;
SemaphoreHandle_t     communication_semaphore;
EventGroupHandle_t    stateEventGroup;


int main(void) {
    
    sensor_data_mutex = xSemaphoreCreateMutex();
    threshold_data_mutex = xSemaphoreCreateMutex();
    state_mutex = xSemaphoreCreateMutex();
    communication_semaphore = xSemaphoreCreateBinary();
    stateEventGroup = xEventGroupCreate();

    setStartupEventBits();
    initIO();
    
    lightSensorSetup();
    _delay_ms(6000);
    
    xTaskCreate(tsk_read_sensors, "read_sensors", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(tsk_humidity_control, "humidity_control", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(tsk_light_control, "light_control", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(tsk_temperature_control, "temperature_control", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    xTaskCreate(tsk_button_control, "button_control", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(tsk_communication, "communication", configMINIMAL_STACK_SIZE, NULL, 0, NULL);

    vTaskStartScheduler();
	return 0;
}

    