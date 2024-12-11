#define F_CPU 3333333

#include "esp32-uart.h"


void initUART() {
    PORTC.DIR &= ~PIN1_bm; // RX pin
    PORTC.DIR |= PIN0_bm; // TX pin
    USART1.BAUD = 1389; // 9600 baud rate
    // standard uart settings
    USART1.CTRLC |= (USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc);
    USART1.CTRLB |= USART_TXEN_bm;
    USART1.CTRLB |= USART_RXEN_bm;
}

void receiveThresholdData(ThresholdData* data) {
    uint8_t buf[6];
    
    // timeout is included to ensure the system doesn't halt if the ESP32 becomes unresponsive
    uint8_t timeout = 0;
    for (uint8_t i = 0; i < 6; i++) {
        while(!(USART1.STATUS & USART_RXCIF_bm) && timeout < 100) {
            timeout++;
            vTaskDelay(pdMS_TO_TICKS(10));;
        }
        if (timeout == 100) {
            return;
        }
        else {
            // receive all user input threshold data from the ESP32
            buf[i] = USART1.RXDATAL;
            while (!(USART1.STATUS & USART_DREIF_bm));
            USART1.TXDATAL = 0x06;
        }
    }
    
    data->light = (buf[0] << 8) | buf[1]; // Light levels are stored in two bytes
    data->maxTemperature = buf[2];
    data->minTemperature = buf[3];
    data->maxHumidity = buf[4];
    data->minHumidity = buf[5];
}

// The ESP32 will send a 1 if the day alarm is triggered and a 2 if the night alarm was triggered
void receiveDayOrNight(uint8_t* buffer) {
    while(!(USART1.STATUS & USART_RXCIF_bm));
    buffer[0] = USART1.RXDATAL;
    // 0x06 is recognized by the ESP32 as an ack that data was received
    while(!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = 0x06;
}


void sendCurrentData(SensorData* data, ThresholdData* thresholds, uint8_t* buf) {
    uint8_t receiveThresholds = 0;
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = 0x01; // This indicates that the atMega3208 wants to send sensor readings
    
    // Send all sensor readings
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = data->currentLight >> 8;
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = data->currentLight & 0xFF;
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = data->currentTemperature;
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = data->currentHumidity;
    
    // Send state information
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = buf[0];
    // Send cooling mode
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = buf[1];
    // Send fan mode
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = buf[2];
    // Send music mode
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = buf[3];
    
    // Receive information about whether or not the RTC alarm was fired
    receiveDayOrNight(buf);
    
    while(!(USART1.STATUS & USART_RXCIF_bm));
    // ESP32 will send a 1 if it has updated threshold settings and 0 if it
    // is bypassing sending threshold setting information
    receiveThresholds = USART1.RXDATAL;
    if (receiveThresholds == 1) {
        receiveThresholdData(thresholds);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
    
}


