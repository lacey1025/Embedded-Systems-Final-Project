/*
 * File:   i2c.c
 * Author: laceysemansky
 *
 * Created on November 1, 2024, 8:35 PM
 */

#define F_CPU 3333333
#include "i2c.h"

#define READ (0x01)
#define WRITE (0x00)


void initTWI() {
    // Init pins and pullup resistors
    PORTA.DIR &= ~(PIN2_bm | PIN3_bm); // Set SDA (PA2) and SCL (PA3) as inputs
    PORTA.PIN2CTRL |= PORT_PULLUPEN_bm; // Enable pull-up resistor on SDA
    PORTA.PIN3CTRL |= PORT_PULLUPEN_bm; // Enable pull-up resistor on SCL

    // Host Initialization
    TWI0.MBAUD = 10; // Set baud rate
    TWI0.MCTRLA |= TWI_ENABLE_bm; // Enable TWI
    TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc; // Set bus state to idle
}

void startWriteOperation(uint8_t address) {
    // Wait until bus is idle
    while (!(TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc));
    // Send address and set R/W bit to 0 for write
    TWI0.MADDR = (address << 1) | WRITE;
    WAIT_FOR_WRITE();
}

void startReadOperation(uint8_t address) {
    // Send address and set R/W bit to 1 for read
    TWI0.MADDR = (address << 1) | READ;
    //WAIT_FOR_WRITE();
}

void endOperation() {
    // Send stop condition
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}

