#include "headers/WifiAndServer.h"
#include "headers/Data.h"
#include "headers/RTC.h"
#include "headers/MusicPlayer.h"
#include "headers/Communication.h"
#include "headers/LcdPeltierFans.h"


void setup() {
  setupUartAndI2C();
  setupWiFi();
  setupLCD();
  setupRTC();
  setupPeltierAndFans();
  startMusicPlayer();
}

void loop() {
  checkForData();
  if (alarmFlag == 1) {
    onTimerExpired();
    alarmFlag = 0;
  }
}