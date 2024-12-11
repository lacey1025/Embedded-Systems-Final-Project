/* 
 * File:   peripherals.h
 * Author: laceysemansky
 *
 * Created on November 19, 2024, 8:55 PM
 */

#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "data.h"
#include "esp32-uart.h"
#include "i2c.h"
#include "light_sensor.h"
#include "temp-humidity-sensor.h"

extern peripheral_t music_button;
extern peripheral_t state_button;
extern peripheral_t water_level_led;
extern peripheral_t grow_light;
extern peripheral_t water_level_switch;
extern peripheral_t mp3_track1;
extern peripheral_t mp3_track2;
extern peripheral_t humidifier;

void initIO();
void initTCA();

#endif	/* PERIPHERALS_H */

