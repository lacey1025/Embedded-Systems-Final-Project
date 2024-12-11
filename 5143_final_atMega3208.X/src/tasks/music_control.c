///*
// * File:   music_control.c
// * Author: laceysemansky
// *
// * Created on November 23, 2024, 8:35 PM
// */
//
//#include "music_control.h"
//
//static uint32_t bits;
//
//void tsk_music_control(void *pvParams) {
//    while (1) {
//        bits = xEventGroupGetBits(stateEventGroup);
//        
//        // you have to pull the pins low to turn the track on
//        if (!(bits & MUSIC_ON)) {
//            if (mp3_track1.state == 0) {
//                mp3_track1.port->OUT |= mp3_track1.bm;
//                mp3_track1.state = 1;
//            }
//            if (mp3_track2.state == 0) {
//                mp3_track2.port->OUT |= mp3_track2.bm;
//                mp3_track2.state = 1;
//            }
//        
//            bits = xEventGroupWaitBits(
//                    stateEventGroup,                 // Event group to wait on
//                    MUSIC_ON,                        // bits to wait on
//                    pdFALSE,                         // don't clear bits after waiting
//                    pdFALSE,                         // don't wait for all bits
//                    portMAX_DELAY);                  // delay indefinitely
//        } else {
//            if (bits & STATE_WATERING_MODE) {
//                if (mp3_track1.state == 0) {
//                    mp3_track1.port->OUT |= mp3_track1.bm;
//                    mp3_track1.state = 1;
//                }
//                if (mp3_track2.state == 1) {
//                    mp3_track2.port->OUT &= ~mp3_track2.bm;
//                    mp3_track2.state = 0;
//                }
//            } else {
//                if (mp3_track2.state == 0) {
//                    mp3_track2.port->OUT |= mp3_track2.bm;
//                    mp3_track2.state = 1;
//                }
//                if (mp3_track1.state == 1) {
//                    mp3_track1.port->OUT &= ~mp3_track1.bm;
//                    mp3_track1.state = 0;
//                }
//            }
//        }
//        vTaskDelay(pdMS_TO_TICKS(100));
//    }
//}
