/* Utilizes the station mode feature of the ESP32 to connect to a local
* WiFi network and run a very small server with one GET and one POST method 
*/
#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "Data.h"
#include "RTC.h"

extern volatile bool wifiEnabled;

// Starts the Wifi capabilities of the ESP32
void setupWiFi();

// Begins the web server and sets up GET and POST methods
void setupWebServer();

// Turn on an off wifi for night and day changes
void turnOffWiFi();
void turnOnWiFi();

// Sends readings collected from the atMega3208 to the smartphone application
void handleGETData(AsyncWebServerRequest *request);

// Collects changes in threshold settings such as light amount, max humidity, etc.
void handlePOSTUpdate(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

#endif