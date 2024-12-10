#include "headers/LcdPeltierFans.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupPeltierAndFans() {
  ledcAttach(PELTIER_PIN, 5000, 8);
  ledcWrite(PELTIER_PIN, 0);

  pinMode(PELTIER_FAN_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void setupLCD() {
  if (!i2cAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3f, 16, 2);
  }
  delay(100);
  lcd.init();
  delay(100);
  lcd.backlight();
  delay(100);
  lcdPrintTempAndHumidity();
}

void lcdPrintTempAndHumidity() {
  SensorData dataCopy;
  if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    dataCopy = sensorData;
    xSemaphoreGive(dataMutex);
  }
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(dataCopy.currentTemperature);
  lcd.print((char)223);
  lcd.print("F ");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(dataCopy.currentHumidity);
  lcd.print("% ");
  delay(100);
}

bool i2cAddrTest(uint8_t addr) {
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    return true;
  }
  return false;
}