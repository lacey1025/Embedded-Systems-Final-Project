/* 
 * File:   light_control.h
 * Author: laceysemansky
 *
 * Created on November 25, 2024, 10:00 AM
 */

#ifndef LIGHT_CONTROL_H
#define	LIGHT_CONTROL_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_light_control(void *pvParams);

#endif	/* LIGHT_CONTROL_H */

