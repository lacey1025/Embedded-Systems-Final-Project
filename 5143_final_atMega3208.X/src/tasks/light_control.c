/*
 * File:   light_control.c
 * Author: laceysemansky
 *
 * Created on November 23, 2024, 8:35 PM
 */

#include "light_control.h"
#define MAX_PWM_VALUE 11109
#define MIN_PWM_VALUE 0

// PID Constants (tune these for your system)
#define P_CONST 1.0
#define I_CONST 0
#define D_CONST 0

static uint16_t desiredLight;
static uint16_t currentLight;
static uint32_t bits;
static int total_error = 0;
static int previous_error = 0;
static int pwm_value = 0;
    

void tsk_light_control(void *pvParams) {
    while (1) {
        bits = xEventGroupGetBits(stateEventGroup);
        
        // If the state the system is not in lights on state, check if the lights
        // are currently on and switch them off if needed.
        // This includes the grow light and the water level led
        if (!(bits & STATE_ALL_ON)) {
            if (grow_light.state == 1) {
                TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; // disable TCA
                vTaskDelay(pdMS_TO_TICKS(50));
                grow_light.port->OUT &= ~grow_light.bm;
                grow_light.port->DIR &= ~grow_light.bm;
                grow_light.state = 0;
            }
            if (water_level_led.state == 1) {
                water_level_led.port->OUT &= ~water_level_led.bm;
                water_level_led.state = 0;
            }
            
            // This will cause the task to block until STATE_ALL_ON is set to 1
            bits = xEventGroupWaitBits(
                    stateEventGroup,                 // Event group to wait on
                    STATE_ALL_ON,                    // bits to wait on
                    pdFALSE,                         // don't clear bits after waiting
                    pdFALSE,                         // don't wait for all bits
                    portMAX_DELAY);                  // delay indefinitely
            
        } else { // State is set to lights all on
            // Safely collect user input light level and sensor light reading
            if (xSemaphoreTake(threshold_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                desiredLight = thresholdData.light;
                xSemaphoreGive(threshold_data_mutex);
            }
            if (xSemaphoreTake(sensor_data_mutex, pdMS_TO_TICKS(500)) == pdTRUE) {
                currentLight = sensorData.currentLight;
                xSemaphoreGive(sensor_data_mutex);
            }

            // Control LED output
            if (!(bits & WATER_LEVEL_OKAY) && (water_level_led.state == 0)) {
                water_level_led.port->OUT |= water_level_led.bm;
                water_level_led.state = 1;
            } else if ((bits & WATER_LEVEL_OKAY) && water_level_led.state == 1) {
                water_level_led.port->OUT &= ~water_level_led.bm;
                water_level_led.state = 0;
            }

            // Turn on grow light if it is off
            if (grow_light.state == 0) {
                initTCA();
                grow_light.state = 1;
            }

            // Create a PID driver to adjust the PWM output of the grow lights
            if (grow_light.state == 1) {
                int error = desiredLight - currentLight;
                total_error += error;
                if (total_error > 10000) {
                    total_error = 10000;
                } else if (total_error < -10000) {
                    total_error = -10000;
                }
                
                int delta_error = error - previous_error;
                
                int drive = (P_CONST * error) +
                    (I_CONST * total_error) +
                    (D_CONST * delta_error);
                
                pwm_value += drive;

                // Constrain PWM value to valid range
                if (pwm_value > MAX_PWM_VALUE) {
                    pwm_value = MAX_PWM_VALUE;
                } else if (pwm_value < MIN_PWM_VALUE) {
                    pwm_value = MIN_PWM_VALUE;
                }
                
                TCA0.SINGLE.CMP1 = pwm_value;

                // Store the current error for the next loop
                previous_error = error;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

