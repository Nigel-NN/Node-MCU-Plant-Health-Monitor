# Plant Health Monitor

## Overview

The Plant Health Monitor is an IoT-based system designed to help you monitor the health of your plants in real-time. It uses an ESP8266 microcontroller to gather data from various sensors and provides a user-friendly web interface to view and manage this data.

## Features

- **Real-time Monitoring**: The system measures soil moisture, light intensity, temperature, and humidity in real-time.
- **Web Interface**: A user-friendly web interface allows you to view the current sensor readings from any device with a web browser.
- **Adjustable Thresholds**: The web interface includes sliders to adjust the desired range of moisture and light levels. The system will interpret the sensor data based on these thresholds.
- **Heartbeat LED**: An LED blinks in a heartbeat pattern to indicate that the system is working properly.

## Components

- **ESP8266 Microcontroller**: This is the main controller used in this project. It reads data from the sensors, hosts the web server, and serves the web interface.
- **Moisture Sensor**: This sensor measures the moisture level in the soil.
- **Light Sensor (LDR)**: This sensor measures the light intensity. It has both digital and analog outputs.
- **DHT11 Sensor**: This sensor measures the temperature and humidity in the environment.
- **LED**: An LED is used to indicate the operation of the system.

## Setup

1. Connect the ESP8266 to your computer and upload the code using the Arduino IDE.
2. Once the code is uploaded, the ESP8266 will start a Wi-Fi access point.
3. Connect your device (computer, phone, etc.) to this Wi-Fi network.
4. Open a web browser and navigate to the IP address of the ESP8266 (printed in the Serial Monitor).
5. You should see a web page with the current sensor readings and sliders to adjust the thresholds.

## Libraries Used

- ESP8266WiFi.h: This library allows an ESP8266 to connect to a local Wi-Fi network.
- ESP8266WebServer.h: This library allows the ESP8266 to host a web server.
- DHT.h: This library allows the ESP8266 to read data from DHT series sensors.

## Future Improvements

- Add more sensors to monitor other parameters such as soil pH and nutrient levels.
- Implement a notification system to alert the user when the sensor readings are out of the desired range.
- Integrate with a cloud platform to store and analyze the sensor data over time.
