/* 
 * File:   i2c.h
 * Author: laceysemansky
 *
 * Created on November 7, 2024, 9:19 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <avr/io.h>
#include <util/delay.h>

#define WAIT_FOR_WRITE() while (!(TWI0.MSTATUS & TWI_WIF_bm))
#define WAIT_FOR_READ() while (!(TWI0.MSTATUS & TWI_RIF_bm))

void initTWI();
void startWriteOperation(uint8_t address);
void startReadOperation(uint8_t address);
void endOperation();

#endif	/* I2C_H */

