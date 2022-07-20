# Safety Manger

## Important links:
1. Safety Manger Website - https://safetymanager-52fc3.web.app/
2. Schedule and summary of meetings during the semester - https://docs.google.com/document/d/1P6jc5bVWA_hnhraxQq3wialfgLse7NmTXBHypVYeOqE/edit
3. Reflashing sonoff26 firmware - https://tasmota.github.io/docs/devices/Sonoff-S26-Smart-Socket/#led-state

## How to use the device:
1. Parameters that are hard coded: 
   a. The sonoff MAC address on the ESP32 code. When uploading the code to the sonoff, it's MAC address will appear on the monitor. The lab     manager should copy this MAC address and define it on line 114.\n
   b. When uploading the code to the ESP32, the lab manager should define in line 20 the machine ID as he defined on the website.
   c. On the ESP32 code - update WiFi parameters (Name and password) on lines 9-10.
2. Connect the sonoff to electricity and connect the machine to the sonoff.
3. Connect the device to electricity.
4. Manage permissions and users through Safety Manger Website.

## Main libraries used in the project:
1. Firebase_ESP_Client.h
2. MFRC522.h
3. espnow.h
4. ESP8266WiFi.h
5. ESP8266WebServer.h
6. ESP8266mDNS.h
7. WiFi.h
