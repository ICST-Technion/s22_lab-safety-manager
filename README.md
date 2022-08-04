# Safety Manger

## How to use the device:
1. Connect the sonoff to electricity and connect the machine to the sonoff.
2. Connect the device to electricity.
3. Manage permissions and users through Safety Manager Website (Notice - level 1 is the highest permission, allows usage of all machines).

## Important links:
1. Safety Manager Website - https://safetymanager-52fc3.web.app/
2. Schedule and summary of meetings during the semester - https://docs.google.com/document/d/1P6jc5bVWA_hnhraxQq3wialfgLse7NmTXBHypVYeOqE/edit
3. Reflashing sonoff26 firmware - https://tasmota.github.io/docs/devices/Sonoff-S26-Smart-Socket/#led-state

## Activate new device:
1. Solder the sonoff as showen on (3) link from above.
2. Upload the sonoff code and copy it's MAC address from the monitor.
3. Upload the ESP32 code change the hard coded parameters as explained below.

## Parameters that are hard coded:
1. The sonoff MAC address on the ESP32 code. When uploading the code to the sonoff, it's MAC address will appear on the monitor. The lab     manager should copy this MAC address and define it on line 114.
2. When uploading the code to the ESP32, the lab manager should define in line 20 the machine ID as he defined on the website.
3. On the ESP32 code - update WiFi parameters (Name and password) on lines 9-10.

## Main libraries used in the project:
1. Firebase_ESP_Client.h - 3.15.5
2. MFRC522.h - 1.4.10
3. espnow.h - 2.0
4. ESP8266WiFi.h - 1.0
5. ESP8266WebServer.h - 1.0
6. ESP8266mDNS.h - 1.1
7. WiFi.h - 1.2.7
