/* This file is used to control the LCD, peltier cooling device, fan attached to the
* peltier, and the fan within the enclosure for humidity control.
*/
#ifndef LCD_PELTIER_FANS.h
#define LCD_PELTIER_FANS.h

#include <LiquidCrystal_I2C.h>
#include "Data.h"

#define PELTIER_PIN 13
#define PELTIER_FAN_PIN 12
#define FAN_PIN 26

extern LiquidCrystal_I2C lcd;

void setupPeltierAndFans();
void setupLCD();
void lcdPrintTempAndHumidity();
bool i2cAddrTest(uint8_t addr);

#endif