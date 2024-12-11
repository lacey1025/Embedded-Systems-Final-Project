/* 
 * File:   data.h
 * Author: laceysemansky
 *
 * Created on November 16, 2024, 7:21 PM
 */

#ifndef DATA_H
#define	DATA_H

#define F_CPU 3333333
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stream_buffer.h"
#include "message_buffer.h"
#include "semphr.h"
#include "event_groups.h"

// Struct for storing current sensor readings
typedef struct {
  volatile uint16_t currentLight;
  volatile uint8_t currentTemperature;
  volatile uint8_t currentHumidity;
} SensorData;

// Struct for storing user defined climate thresholds
typedef struct {
  volatile uint16_t light;
  volatile uint8_t maxTemperature;
  volatile uint8_t minTemperature;
  volatile uint8_t maxHumidity;
  volatile uint8_t minHumidity;
} ThresholdData;

// Useful peripheral data
typedef struct {
	PORT_t *port;
	uint8_t bm;
    volatile uint8_t state;
} peripheral_t;

// Event bit group definitions
#define STATE_ALL_ON        (1 << 0)
#define STATE_LIGHTS_OFF    (1 << 1)
#define STATE_NIGHT_MODE    (1 << 2)
#define MUSIC_ON            (1 << 3)
#define WATER_LEVEL_OKAY    (1 << 4)
#define STATE_WATERING_MODE (1 << 5)
#define STATE_COOLING_MODE  (1 << 6)
#define FAN_ON              (1 << 7)
#define MUSIC_PREVIOUSLY_ON (1 << 8)

extern ThresholdData thresholdData;
extern SensorData sensorData;

// Mutexes for safely reading and writing to shared variables
extern SemaphoreHandle_t     sensor_data_mutex;
extern SemaphoreHandle_t     threshold_data_mutex;
extern SemaphoreHandle_t     state_mutex;

// This semaphore ensures communication only occurs after new readings are collected from sensors
extern SemaphoreHandle_t     communication_semaphore;

// Event group to store state flags. This is a really useful feature in freeRTOS that guarantees
// safe reads and writes. Also comes with functions to wait for bits to be set
extern EventGroupHandle_t    stateEventGroup;

// This function sets and clears the necessary bit flags for default startup configuration
void setStartupEventBits();

#endif	/* DATA_H */

