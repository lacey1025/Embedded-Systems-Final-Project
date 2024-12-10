#include "headers/WifiAndServer.h"

const char* ssid = "*******";
const char* password = "********";
volatile bool wifiEnabled = true;
AsyncWebServer server(80);

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Start mDNS
  MDNS.begin("esp32");
  setupWebServer();
}

void setupWebServer() {
  server.on("/data", HTTP_GET, handleGETData);
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"status\": \"received\"}");
  }, nullptr, handlePOSTUpdate);

  server.begin();
}

void turnOffWiFi() {
    server.end();
    MDNS.end();
    delay(100);
    WiFi.disconnect(true);
    delay(100);
    wifiEnabled = false;
}

void turnOnWiFi() {
  setupWiFi();
  setupWebServer();
  wifiEnabled = true;
}

void handleGETData(AsyncWebServerRequest *request) {
  String json;
  if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    json = "{\"currentLight\": " + String(sensorData.currentLight) +
                  ", \"currentHumidity\": " + String(sensorData.currentHumidity) +
                  ", \"currentTemperature\": " + String(sensorData.currentTemperature);
    xSemaphoreGive(dataMutex);
  } else {
    request->send(500, "application/json", "{\"error\":\"Operation failed. Please try again.\"}");
    return;
  }
  if (xSemaphoreTake(lightMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    json += ", \"lightOnTime\": \"" + String(lightData.lightOnTime) + "\"" +
                  ", \"lightOffTime\": \"" + String(lightData.lightOffTime) + "\"";
    xSemaphoreGive(lightMutex);
  } else {
    request->send(500, "application/json", "{\"error\":\"Operation failed. Please try again.\"}");
    return;
  }
  if (xSemaphoreTake(thresholdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    json += ", \"light\": " + String(thresholdData.light) +
                  ", \"maxHumidity\": " + String(thresholdData.maxHumidity) +
                  ", \"minHumidity\": " + String(thresholdData.minHumidity) +
                  ", \"maxTemperature\": " + String(thresholdData.maxTemperature) +
                  ", \"minTemperature\": " + String(thresholdData.minTemperature) + "}";
    xSemaphoreGive(thresholdMutex);
  } else {
    request->send(500, "application/json", "{\"error\":\"Operation failed. Please try again.\"}");
    return;
  }
  request->send(200, "application/json", json);
}

void handlePOSTUpdate(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  String bodyContent;
  for (size_t i = 0; i < len; i++) {
      bodyContent += (char)data[i];
  }

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, bodyContent);
  String lightOnTime;
  String lightOffTime;

  if (error) {
    request->send(400, "application/json", "{\"status\": \"failed to parse JSON\"}");
    return;
  }
  if (xSemaphoreTake(lightMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    // Update threshold settings if they exist in JSON
    if (doc.containsKey("lightOnTime")) {
        lightOnTime = doc["lightOnTime"].as<String>();
        lightData.lightOnTime = lightOnTime;
    }
    if (doc.containsKey("lightOffTime")) {
        lightOffTime = doc["lightOffTime"].as<String>();
        lightData.lightOffTime = lightOffTime;
    }
    xSemaphoreGive(lightMutex);
  } else {
    request->send(500, "application/json", "{\"error\":\"Operation failed. Please try again.\"}");
    return;
  }
  if (xSemaphoreTake(thresholdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    if (doc.containsKey("light")) {
        thresholdData.light = doc["light"].as<int>();
    }
    if (doc.containsKey("maxTemperature")) {
        thresholdData.maxTemperature = doc["maxTemperature"].as<int>();
    }
    if (doc.containsKey("minTemperature")) {
        thresholdData.minTemperature = doc["minTemperature"].as<int>();
    }
    if (doc.containsKey("maxHumidity")) {
        thresholdData.maxHumidity = doc["maxHumidity"].as<int>();
    }
    if (doc.containsKey("minHumidity")) {
        thresholdData.minHumidity = doc["minHumidity"].as<int>();
    }
    xSemaphoreGive(thresholdMutex);
  } else {
    request->send(500, "application/json", "{\"error\":\"Operation failed. Please try again.\"}");
    return;
  }

  request->send(200, "application/json", "{\"status\": \"success\"}");

  setAlarms();

  needToSendThreshold = true;
}