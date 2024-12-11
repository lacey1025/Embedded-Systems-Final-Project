/*
 * File:   temp-humidity-sensor.c
 * Author: laceysemansky
 *
 * Created on November 9, 2024, 8:35 PM
 */

#define F_CPU 3333333
#include "temp-humidity-sensor.h"

#define SENSOR_ADDR (0x44)

void getRawData(uint32_t *buf) {
    uint8_t data[6];
    startWriteOperation(SENSOR_ADDR);
    WAIT_FOR_WRITE();
    TWI0.MDATA = 0x24; // Clock stretching
    WAIT_FOR_WRITE();
    TWI0.MDATA = 0x00; // medium repeatability
    WAIT_FOR_WRITE();
    endOperation();
    vTaskDelay(pdMS_TO_TICKS(20));
    
    startReadOperation(SENSOR_ADDR);
    
    // Collect sensor readings: 2 bytes for temperature, 1 byte checksum, 2 bytes humidity, 1 byte checksum
    for (uint8_t i=0; i<6; i++) {
        WAIT_FOR_READ();
        data[i] = TWI0.MDATA;
        if (i < 5) {
            // Send ACK and receive more data
            TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
        } else {
            // Send NACK and stop transmission
            TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
        }
    }
    
    // Combine the temperature and humidity bytes
    buf[0] = ((uint32_t)data[0] << 8) | data[1];
    buf[1] = ((uint32_t)data[3] << 8) | data[4];
}

void getTempAndHumidity(uint16_t *buf) {
    uint32_t raw_data[2];
    getRawData(raw_data);
    // Conversion formula for temperature from the datasheet 
    // made to work better with the microcontroller
    uint32_t temp_f = ((7875 * raw_data[0] + (1 << 13)) >> 14) - 4900;
    buf[0] = (uint16_t)(temp_f / 100);
    
    // Conversion formula for humidity from raw data
    uint32_t shum = (uint32_t)raw_data[1];
    shum = (100 * shum + (1 << 15)) >> 16;
    buf[1] = (uint16_t)shum;
}

uint16_t getTemperature() {
    uint32_t raw_data[2];
    getRawData(raw_data);
    
    uint32_t temp_f = ((7875 * raw_data[0] + (1 <<13)) >> 14) - 4900;
    return (uint16_t)(temp_f / 100);\
}

uint16_t getHumidityPercentage() {
    uint32_t raw_data[2];
    getRawData(raw_data);
    
    uint32_t shum = ((uint32_t)raw_data[1]);
    shum = (100 * shum) / 65535;
    if (shum > 100) {
        shum = 100;
    }
    return (int16_t)shum;
}


