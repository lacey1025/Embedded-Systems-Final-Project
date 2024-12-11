/* 
 * File:   temperature_control.h
 * Author: laceysemansky
 *
 * Created on November 25, 2024, 2:55 PM
 */

#ifndef TEMPERATURE_CONTROL_H
#define	TEMPERATURE_CONTROL_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_temperature_control(void *pvParams);

#endif	/* TEMPERATURE_CONTROL_H */

