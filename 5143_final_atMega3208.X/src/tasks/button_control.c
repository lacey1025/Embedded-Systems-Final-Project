/*
 * File:   button_control.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "button_control.h"
   
uint32_t bits;

void tsk_button_control(void *pvParams) {
    while (1) {
        // Collect information on whether either button is pressed
        music_button.state = !(music_button.port->IN & music_button.bm);
        state_button.state = !(state_button.port->IN & state_button.bm);  
        bits = xEventGroupGetBits(stateEventGroup); // get current bit flags
        
        // Music button is pressed, turn on music if it was off or vice versa
        if (music_button.state == 1) {
            if (!(bits & MUSIC_ON)) {
                xEventGroupSetBits(stateEventGroup, MUSIC_ON | MUSIC_PREVIOUSLY_ON);
            } else {
                xEventGroupClearBits(stateEventGroup, MUSIC_ON | MUSIC_PREVIOUSLY_ON);
            }
        }
        
        // State button was pressed, toggle to next state: all on, lights off but everything else
        // on, or everything off
        if (state_button.state == 1) {
            // Ensure only button or ESP32 data communicator are updating the state at once
            if (xSemaphoreTake(state_mutex, portMAX_DELAY) == pdTRUE) {
                if (bits & STATE_ALL_ON) {
                    xEventGroupClearBits(stateEventGroup, STATE_ALL_ON);
                    xEventGroupSetBits(stateEventGroup, STATE_LIGHTS_OFF);
                } else if (bits & STATE_LIGHTS_OFF) {
                    xEventGroupClearBits(stateEventGroup, STATE_LIGHTS_OFF | MUSIC_ON);
                    xEventGroupSetBits(stateEventGroup, STATE_NIGHT_MODE);
                } else {
                    xEventGroupClearBits(stateEventGroup, STATE_NIGHT_MODE);
                    xEventGroupSetBits(stateEventGroup, STATE_ALL_ON);
                    
                    // only turn the music on if it was on before night mode was entered
                    if (bits & MUSIC_PREVIOUSLY_ON) {
                        xEventGroupSetBits(stateEventGroup, MUSIC_ON);
                    }
                }
                xSemaphoreGive(state_mutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(125));
    }
}
