#include "headers/RTC.h"

RTC_DS3231 rtc;

void setupRTC() {
  if (!rtc.begin(&Wire)) {
    return;
  }
  if(rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.begin(&Wire);
  rtc.disable32K();
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), alarmISR, FALLING);

  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  rtc.writeSqwPinMode(DS3231_OFF);
  setAlarms();
}

void setAlarms() {
  int onHour, onMinute, offHour, offMinute;
  if (xSemaphoreTake(lightMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    onHour = lightData.lightOnTime.substring(0, 2).toInt();
    onMinute = lightData.lightOnTime.substring(2, 4).toInt();
    offHour = lightData.lightOffTime.substring(0, 2).toInt();
    offMinute = lightData.lightOffTime.substring(2, 4).toInt();
    xSemaphoreGive(lightMutex);
  } else {
    return;
  }

  DateTime now = rtc.now();
  // rtc is about 50 sec behind because it got time from computer during upload but upload took 50 seconds. We'll just adjust here for now
  int32_t currentTimeInSeconds = (now.hour() * 3600) + (now.minute() * 63) + now.second() + 30;
  int32_t onTimeInSeconds = onHour * 3600 + onMinute * 60;
  int32_t offTimeInSeconds = offHour * 3600 + offMinute * 60;

  // Calculate countdowns for both ON and OFF alarms
  int32_t lightOnCountdown = onTimeInSeconds - currentTimeInSeconds;
  if (lightOnCountdown <= 0) {
    lightOnCountdown += 24 * 3600; 
  }

  int32_t lightOffCountdown = offTimeInSeconds - currentTimeInSeconds;
  if (lightOffCountdown <= 0) {
    lightOffCountdown += 24 * 3600;
  }


 
  if (offTimeInSeconds < currentTimeInSeconds) {
    // It's daytime, but the OFF time is in the past -> Switch to night
    currentState = NIGHT;
    dayChange = 1;
    if (lcdBacklightIsOn == true) {
      lcd.noBacklight();
      lcdBacklightIsOn = false;
    }
    
 
  } else if (onTimeInSeconds < currentTimeInSeconds) {
    // It's nighttime, but the ON time is in the past -> Switch to day
    currentState = DAY;
    dayChange = 2;
    if (lcdBacklightIsOn == false) {
      lcd.backlight();
      lcdBacklightIsOn = true;
    }
  }

  // Set Alarm 1 for Light ON
  DateTime alarmOnTime = now + TimeSpan(lightOnCountdown);
  rtc.setAlarm1(alarmOnTime, Ds3231Alarm1Mode::DS3231_A1_Hour);

  // Set Alarm 2 for Light OFF
  DateTime alarmOffTime = now + TimeSpan(lightOffCountdown);
  rtc.setAlarm2(alarmOffTime, Ds3231Alarm2Mode::DS3231_A2_Hour);
}

void onTimerExpired() {
  if (rtc.alarmFired(1)) {
      currentState = DAY;
      dayChange = 2;
      if (lcdBacklightIsOn == false) {
        lcd.backlight();
        lcdBacklightIsOn = true;
      }
      
      if (wifiEnabled == false) {
        turnOnWiFi();
      }
      rtc.clearAlarm(1);
  }
  else if (rtc.alarmFired(2)) {
    currentState = NIGHT;
    dayChange = 1;
    if (lcdBacklightIsOn == true) {
      lcd.noBacklight();
      lcdBacklightIsOn = false;
    }
    
    if (wifiEnabled == true) {
      turnOffWiFi();
    }
    rtc.clearAlarm(2);
  }
}

void alarmISR() {
  alarmFlag = 1;
}