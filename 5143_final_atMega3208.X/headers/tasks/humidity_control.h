/* 
 * File:   humidity_control.h
 * Author: laceysemansky
 *
 * Created on November 24, 2024, 9:49 PM
 */

#ifndef HUMIDITY_CONTROL_H
#define	HUMIDITY_CONTROL_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_humidity_control(void *pvParams);

#endif	/* HUMIDITY_CONTROL_H */

