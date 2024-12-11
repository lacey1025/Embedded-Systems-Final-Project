/*
 * File:   light_sensor.c
 * Author: laceysemansky
 *
 * Created on November 1, 2024, 8:35 PM
 */

#define F_CPU 3333333

#include "light_sensor.h"


void lightSensorSetup() {
    startWriteOperation(LIGHT_SENSOR_ADDR);
    // Send command to modify the enable register
    TWI0.MDATA = (COMMAND_BIT | COMMAND_NORMAL_OPERATION | REGISTER_ENABLE);
    WAIT_FOR_WRITE();
    // Write power on in register enable
    TWI0.MDATA = (ENABLE_POWERON | ENABLE_AEN);
    WAIT_FOR_WRITE();
    endOperation();
    _delay_ms(100);
    setGainAndIntegration();
}

void lightSensorDisable() {
    startWriteOperation(LIGHT_SENSOR_ADDR);
    // Send command to edit enable register
    TWI0.MDATA = (COMMAND_BIT | COMMAND_NORMAL_OPERATION | REGISTER_ENABLE);
    WAIT_FOR_WRITE();
    // Write power off in register enable
    TWI0.MDATA = ENABLE_POWEROFF;
    WAIT_FOR_WRITE();
    endOperation();
}

void setGainAndIntegration() {
    startWriteOperation(LIGHT_SENSOR_ADDR);
    // Request control register for write operation
    TWI0.MDATA = (COMMAND_BIT | COMMAND_NORMAL_OPERATION | REGISTER_CONTROL);
    WAIT_FOR_WRITE();
    // Set gain to medium, lowest integration setting - may need to change
    // Integration time is a bit like shutter speed in a camera
    TWI0.MDATA = (GAIN_LOW | INTEGRATIONTIME_100MS);
    WAIT_FOR_WRITE();
    endOperation();
}

uint16_t getLightSensorReading() {
    uint32_t data = getFullLuminosity();
    return calculateLux(data);
}

uint32_t getFullLuminosity() {
    uint16_t data[4];
    
    startWriteOperation(LIGHT_SENSOR_ADDR);
    // Wait for ADC to complete (A bit longer than our 100ms integration)
    vTaskDelay(pdMS_TO_TICKS(120));
    // Send register channel 0 low to be read
    TWI0.MDATA = (COMMAND_BIT | COMMAND_NORMAL_OPERATION | REGISTER_CHAN0_LOW);
    WAIT_FOR_WRITE();
    // Repeat start condition
    TWI0.MCTRLB |= TWI_MCMD_REPSTART_gc;
    
    startReadOperation(LIGHT_SENSOR_ADDR);
    
    for (uint8_t i=0; i<4; i++) {
        // Wait for read to finish
        WAIT_FOR_READ();
        // Assign dest[i] to read data
        data[i] = TWI0.MDATA;
        if (i < 3) {
            // Send ACK and receive more data
            TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
        } else {
            // Send NACK and stop transmission
            TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
        }
    }
    // y is channel 0 which is full spectrum (visible and infrared)
    uint32_t y = (data[1] << 8) | data[0];
    // x is channel 1 which is just infrared
    uint32_t x = (data[3] << 8) | data[2];
    return (x << 16) | y;
}

// This formula is from Adafruit's TSL2591 Library
uint16_t calculateLux(uint32_t rawdata) {
    uint16_t ch0 = rawdata & 0xFFFF;
    uint16_t ch1 = rawdata >> 16;
    
    int32_t diff = (int32_t)ch0 - (int32_t)ch1;
    if (diff < 0 || ch0 == 0) {
        return 0;
    }
    uint32_t ratio = ((uint32_t)ch1 * 1000) / (uint32_t)ch0;
    uint32_t lux = (diff * (1000 - ratio)) / 245;
    if (lux > 65535) {
        lux = 65535;
    }
    return (uint16_t)lux;
}


