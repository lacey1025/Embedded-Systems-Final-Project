/* This file stores global variables used by the ESP32 system.
* These include structs for shared data with the atMega3208, various state
* flags, and mutexes to ensure the async web feature does not cause race condtions.
*/
#ifndef DATA_H
#define DATA_H

#include <Wire.h>
#include <cstdint>
#include <HardwareSerial.h>

#define RXD2 32
#define TXD2 33

// Sensor variables
typedef struct {
  volatile uint16_t currentLight = 0;
  volatile uint8_t currentTemperature = 25;
  volatile uint8_t currentHumidity = 60;
} SensorData;

// Time data for when the system should go to sleep and wake up
typedef struct {
  String lightOnTime = "0630";
  String lightOffTime = "2000";
} LightData;

// Threshold data collected from the smartphone application used to control the system
typedef struct {
  volatile uint16_t light = 1000;
  volatile uint8_t maxTemperature = 30;
  volatile uint8_t minTemperature = 15;
  volatile uint8_t maxHumidity = 70;
  volatile uint8_t minHumidity = 30;
} ThresholdData;

// Enum for determining current day or night state
typedef enum {
  DAY,
  NIGHT
} CurrentState;

extern SensorData sensorData;
extern ThresholdData thresholdData;
extern LightData lightData;

// Flags for keeping state in check
extern volatile CurrentState currentState;
extern volatile uint8_t alarmFlag;
extern volatile bool readingCurrentData;
extern volatile bool needToSendThreshold;
extern volatile bool lcdBacklightIsOn;
extern volatile bool coolingIsOn;
extern volatile bool fanIsOn;
extern volatile uint8_t dayChange;
extern volatile uint8_t musicState;

// Mutexes to ensure safe data read and writes
extern SemaphoreHandle_t dataMutex;
extern SemaphoreHandle_t thresholdMutex;
extern SemaphoreHandle_t lightMutex;

// Initializes I2C and UART communcation
void setupUartAndI2C();

#endif