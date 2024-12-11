/*
 * File:   peripherals.c
 * Author: laceysemansky
 *
 * Created on November 1, 2024, 8:35 PM
 */

#define F_CPU 3333333

#include "peripherals.h"
#define MAX_PWM_VALUE 11109


peripheral_t humidifier = { .port = &PORTA, .bm = PIN6_bm, .state = 0 };
peripheral_t water_level_led = { .port = &PORTA, .bm = PIN7_bm, .state = 0 };

peripheral_t grow_light = { .port = &PORTD, .bm = PIN1_bm, .state = 1 };
peripheral_t music_button = { .port = &PORTD, .bm = PIN5_bm, .state = 0 };
peripheral_t water_level_switch = { .port = &PORTD, .bm = PIN6_bm, .state = 0 };
peripheral_t state_button = { .port = &PORTD, .bm = PIN7_bm, .state = 0 };

    
void initIO() {
    // These items input
    PORTD.DIR &= ~(music_button.bm | state_button.bm | water_level_switch.bm);
    
    // These items are output
    PORTA.DIR |= water_level_led.bm | humidifier.bm;
    
    // Pullup resistors for buttons and switch
    music_button.port->PIN5CTRL |= PORT_PULLUPEN_bm;
    state_button.port->PIN7CTRL |= PORT_PULLUPEN_bm;
    water_level_switch.port->PIN6CTRL |= PORT_PULLUPEN_bm;
    
    initUART();
    initTWI();
    initTCA();
}

void initTCA() {
    grow_light.port->DIR |= grow_light.bm;
    // Send output to port D
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
    // Enable timer A to single slope PWM mode
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    // Set period register value
    TCA0.SINGLE.PER = 11109;
    // Set the compare register value
    TCA0.SINGLE.CMP1 = 11109;
    // Start timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1_gc;
    grow_light.port->OUT |= grow_light.bm;
}


