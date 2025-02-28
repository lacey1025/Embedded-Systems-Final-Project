# 5143 Embedded Systems Final Project
This project involves the development of a smart enclosure designed to provide ideal environmental conditions for growing orchids. The system integrates multiple sensors, actuators, and microcontrollers (ESP32 and Atmega3208) to monitor, control, and automate various parameters such as light, temperature, humidity, and watering schedules. Key features include:

- **Environmental Monitoring:** Real-time measurement of temperature, humidity, and light levels using sensors.
- **Actuator Control:** Automated control of LED lights, fans, and humidifiers to maintain optimal conditions.
- **Modes of Operation:** Configurable day/night modes and an lights-off mode managed by an RTC (Real-Time Clock).
- **Music Playback:** A DY-SV5W MP3 module provides ambient sound for the enclosure.
- **Smartphone Integration:** A custom Android app communicates with the ESP32 via Wi-Fi, allowing users to monitor data and update thresholds for environmental parameters.
- **Watering Mode:** A complex watering sequence is controlled through task delays for precise execution.
- **FreeRTOS Implementation:** System tasks are organized using FreeRTOS for better multitasking and reliability.
- **User Interaction:** Button inputs enable direct control of the system's functionalities.

This project combines embedded systems, IoT, and smart automation to create a unique solution for plant care experimentation, while also serving as a platform for further innovations in controlled-environment horticulture.

## <a href="https://github.com/lacey1025/Embedded-Systems-Final-Project/blob/main/project%20report.pdf">Full Report</a>

## Project Image
<img src="https://github.com/lacey1025/Embedded-Systems-Final-Project/blob/main/plant-enclosure.jpeg" alt="Plant Enclosure" width="400">

