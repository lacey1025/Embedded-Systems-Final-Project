/* 
 * File:   music_control.h
 * Author: laceysemansky
 *
 * Created on November 25, 2024, 6:33 PM
 */

#ifndef MUSIC_CONTROL_H
#define	MUSIC_CONTROL_H

#define F_CPU 3333333

#include "peripherals.h"
#include "data.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void tsk_music_control(void *pvParams);


#endif	/* MUSIC_CONTROL_H */

