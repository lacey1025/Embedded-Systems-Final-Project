#include "headers/Data.h"

SensorData sensorData;
ThresholdData thresholdData;
LightData lightData;
volatile CurrentState currentState = DAY;
volatile uint8_t alarmFlag = 0;
volatile bool readingCurrentData = 0;
volatile bool needToSendThreshold = false;
volatile bool lcdBacklightIsOn = false;
volatile bool coolingIsOn = false;
volatile bool fanIsOn = false;
volatile uint8_t dayChange = 0;
volatile uint8_t musicState = 1;

SemaphoreHandle_t dataMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t thresholdMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t lightMutex = xSemaphoreCreateMutex();

void setupUartAndI2C() {
    // I2C for RTC and LCD
  Wire.begin();
  // UART1 for atMega
  Serial1.begin(9600, SERIAL_8N1);
  // UART2 for music player
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  delay(1000);
}