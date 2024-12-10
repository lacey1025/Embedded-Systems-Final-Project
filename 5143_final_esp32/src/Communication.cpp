#include "headers/Communication.h"

bool sendByte(uint8_t byte) {
  Serial1.write(byte);
  Serial1.flush();
  while (!Serial1.available()) {}
  uint8_t ack = Serial1.read();
  if (ack != 0x06) {
    return false;
  }
  return true;
}

void sendThresholdData() {
  ThresholdData localThresholdData;

  if (xSemaphoreTake(thresholdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    localThresholdData = thresholdData;
    xSemaphoreGive(thresholdMutex);
  }

  sendByte(localThresholdData.light >> 8);
  sendByte(localThresholdData.light & 0xFF);
  sendByte(localThresholdData.maxTemperature);
  sendByte(localThresholdData.minTemperature);
  sendByte(localThresholdData.maxHumidity);
  sendByte(localThresholdData.minHumidity);
}

/* The buffer here is used for various communication
* [0 - 3]: Sensor readings
* [4]: Lights on, lights off, or night mode
* [5]: Cooling mode
* [6]: Fan on or off
* [7]: Music off, track 1, or track 2
*/
void receiveCurrentData() {
  uint8_t buf[8];

  // Read all data from atMega
  for (uint8_t i = 0; i < 8; i++) {
    while (!Serial1.available());
    buf[i] = Serial1.read();
  }

  // If RTC alarm fired, send to atMega
  sendByte(dayChange);
  
  // If data gathered from smartphone application
  if (needToSendThreshold) {
    Serial1.write(0x01);
    Serial1.flush();
    sendThresholdData();
    needToSendThreshold = false;
  } else {
    Serial1.write(0x00);
    Serial1.flush();
  }

  // Update local sensor readings
  if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    sensorData.currentLight = (buf[0] << 8) | buf[1];
    sensorData.currentTemperature = buf[2];
    sensorData.currentHumidity = buf[3];
    xSemaphoreGive(dataMutex);
  } else {
    return;
  }

  // adjust day, night, or lights off only if RTC alarm wasn't fired
  if (dayChange == 0) {
    if (buf[4] == 0) {
      if (lcdBacklightIsOn == false) {
        lcd.backlight();
        lcdBacklightIsOn = true;
      }
      if (wifiEnabled == false) {
        turnOnWiFi();
      }
    } else if (buf[4] == 1) {
      if (lcdBacklightIsOn == true) {
        lcd.noBacklight();
        lcdBacklightIsOn = false;
      }
    } else if (buf[4] == 2) {
      turnOffWiFi();
      if (lcdBacklightIsOn == true) {
        lcd.noBacklight();
        lcdBacklightIsOn = false;
      }
      if (wifiEnabled == true) {
        turnOffWiFi();
      }
    }
  } else {
    dayChange = 0;
  }
  
  // adjust cooling
  if (buf[5] == 1) {
    if (coolingIsOn == false) {
      digitalWrite(PELTIER_FAN_PIN, HIGH);
      ledcWrite(PELTIER_PIN, 128);
      coolingIsOn = true;
    }
  } else if (buf[5] == 0) {
    if (coolingIsOn == true) {
      digitalWrite(PELTIER_FAN_PIN, LOW);
      ledcWrite(PELTIER_PIN, 0);
      coolingIsOn = false;
    }
  }
  // adjust fan
  if (buf[6] == 1 || buf[5] == 1) {
    if (fanIsOn == false) {
      digitalWrite(FAN_PIN, HIGH);
      fanIsOn = true;
    }
  } else if (buf[6] == 0 && buf[5] == 0) {
    if (fanIsOn == true) {
      digitalWrite(FAN_PIN, LOW);
      fanIsOn = false;
    }
  }
  if (buf[7] != musicState) {
    if (buf[7] == 0) {
      stopPlaying();
      musicState = 0;
    } else if (buf[7] == 1) {
      stopPlaying();
      playTrack(1);
      musicState = 1;
    } else if (buf[7] == 2) {
      stopPlaying();
      playTrack(2);
      musicState = 2;
    }
  }
}

uint8_t checkForData() {
  if (Serial1.available() > 0) {
    uint8_t command = Serial1.read();
    switch (command) {
      case 1:
        receiveCurrentData();
        lcdPrintTempAndHumidity();
        return 1;
      case 2:
        if (wifiEnabled = true) {
          turnOffWiFi();
        } else {
          turnOnWiFi();
        }
        return 2;
      default:
        return 0;
    }
  }
  return 0;
}

