/* 
 * File:   communication.h
 * Author: laceysemansky
 *
 * Created on November 25, 2024, 7:25 PM
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_communication(void *pvParams);


#endif	/* COMMUNICATION_H */

