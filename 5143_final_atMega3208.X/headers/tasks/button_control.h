/* 
 * File:   button_control.h
 * Author: laceysemansky
 *
 * Created on November 25, 2024, 7:02 PM
 */

#ifndef BUTTON_CONTROL_H
#define	BUTTON_CONTROL_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_button_control(void *pvParams);


#endif	/* BUTTON_CONTROL_H */

