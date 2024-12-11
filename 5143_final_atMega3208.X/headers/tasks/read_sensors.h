/* 
 * File:   sensor_task.h
 * Author: laceysemansky
 *
 * Created on November 24, 2024, 9:18 PM
 */

#ifndef READ_SENSORS_H
#define	READ_SENSORS_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_read_sensors(void *pvParams);

#endif	/* READ_SENSORS_H */

