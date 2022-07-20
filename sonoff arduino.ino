#include "FS.h"

bool spiffsOK = false;

/* ESP Now Related*/
#include <ESP8266WiFi.h>
#include "espnow.h"
// declaration of callback functions
void OnDataSent(const uint8_t *mac_addr, uint8_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);


// MAC addresses
#define MAC_LEN 6
#define ESP_NOW_MAX_DATA_LEN       250
uint8_t masterMACaddress[MAC_LEN] ; // Main control board
uint8_t tempMACaddress[MAC_LEN];

uint8_t * targetMACaddress;
String deviceMAC;
bool extractMAC(String * rawMAC, uint8_t * MACarray);
bool espNowSendDataToPeer(uint8_t * macAddress, char * dataToSend, unsigned int retryCounter);
bool powerOn = true, masterMacValid = false;
int  dataSent = 0;
unsigned char gpio13Led = 13, gpio12Relay = 12, button = 0;

#ifdef ENABLE_WIFI
// WiFi part
//#include <WiFi.h>
//#include <ESPmDNS.h>
//#include <WebServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
void handle_OnConnect();
void wifiSetup_page();
void handle_NotFound();
bool wifiSetupPageVisited = false;
#endif

// the setup function runs once when you press reset or power the board
void setup()
{
              // Setup the IOs
              pinMode(gpio13Led, OUTPUT);
              digitalWrite(gpio13Led, LOW);

              pinMode(gpio12Relay, OUTPUT);
              digitalWrite(gpio12Relay, LOW);

              pinMode(button, INPUT);

              Serial.begin(115200);
              // Must have for espNow
              WiFi.disconnect();
              WiFi.mode(WIFI_STA);
              WiFi.channel(0);

              delay(500);

              spiffsOK = false;
              if (SPIFFS.begin())
              {
                             Serial.println("SPIFFS mounted");
                             spiffsOK = true;
              }
              else
              {
                             if (SPIFFS.format())
                             {
                                           printf("SPIFFS formated\n");
                                           if (SPIFFS.begin())
                                           {
                                                          printf("SPIFFS mounted\n");
                                                          spiffsOK = true;
                                           }
                             }
              }
              
              if (spiffsOK == false)
              {
                             printf("SPIFFS error!\n");
              }
              if (spiffsOK)
              {
                             File file;
                             String masterMAC;

                             file = SPIFFS.open("/master.txt", "r");
                             if (file)
                             {
                                           masterMAC = file.readString();
                                           printf("masterMAC - %s\n", masterMAC.c_str());
                                           file.close();
                                           if (extractMAC(&masterMAC, masterMACaddress))
                                           {
                                                          masterMacValid = true;
                                           }
                             }
              }

              Serial.print("ESP8266 Board MAC Address:  ");
              Serial.println(WiFi.macAddress());

#ifdef ENABLE_WIFI
#define WIFI_TIMEOUT 20 // 10 seconds
              // WiFi Part
              WiFi.mode(WIFI_AP_STA);
              if (!WiFi.softAP("sonoff8266AP", "12345678"))
              {
                             Serial.println("Failed to init WiFi AP");
              }
              else
              {
                             Serial.println("IP address of AP is:");
                             Serial.println((WiFi.softAPIP()));
              }
              if (!MDNS.begin("esp32"))
              {
                             printf("Error setting up MDNS responder!\n");
              }
              else
                             printf("mDNS responder started\n");
              MDNS.addService("http", "tcp", 80);
              // Web pages setup
              server.on("/", handle_OnConnect);
              server.on("/wifiSetupSelected", wifiSetup_page);
              server.onNotFound(handle_NotFound);
              server.begin();

              spiffsOK = false;
              if (SPIFFS.begin())
              {
                             Serial.println("SPIFFS mounted");
                             spiffsOK = true;
              }
              else
              {
                             if (SPIFFS.format())
                             {
                                           printf("SPIFFS formated\n");
                                           if (SPIFFS.begin())
                                           {
                                                          printf("SPIFFS mounted\n");
                                                          spiffsOK = true;
                                           }
                             }
              }
              if (spiffsOK == false)
              {
                             printf("SPIFFS error!\n");
              }

              // Init ESP-NOW
#endif
              if (esp_now_init() != 0) 
              {
                             Serial.println("Error initializing ESP-NOW");
              }

              // Set ESP-NOW
              esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
              esp_now_register_recv_cb(OnDataRecv);
              esp_now_register_send_cb(OnDataSent);
              
              // Register peer
              //if (esp_now_add_peer(peer01MACaddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0) != 0)
              //           printf("Failed to register peer\n");
}

#ifdef ENABLE_WIFI
String wifiName = "TPLINK", wifiPass = "";
bool connectToWiFiSTA(const char * name, const char * pass);
#endif
bool dataRecv = false;
char data[256];

void loop()
{
              int numOfClientsConnected;
              //lcd.loadFonts(ORBITRON_LIGHT24);
              File file;
              char tempStr[32];
              bool wifiBeginCalled = false, IO3set = true;
#ifdef ENABLE_WIFI
              if (spiffsOK)
              {
                             file = SPIFFS.open("/wifiSetup.txt", "r");
                             if (file)
                             {
                                           wifiName = file.readStringUntil('\n');
                                           printf("wifi name: %s\n", wifiName.c_str());
                                           wifiPass = file.readStringUntil('\n');
                                           printf("wifi pass: %s\n", wifiPass.c_str());
                                           file.close();
                             }
              }
              connectToWiFiSTA(wifiName.c_str(), wifiPass.c_str());
              wifiBeginCalled = true;
#endif
#ifdef ENABLE_WIFI
              while (1)
              {
                             numOfClientsConnected = WiFi.softAPgetStationNum();
                             // If WiFi is not connected and we have a user connected to our AP, trying to connect keeps changing the WiFi channel and will cause disconnectation of the user from our AP
                             if ((numOfClientsConnected > 0) && (WiFi.status() != WL_CONNECTED))
                             {
                                           WiFi.disconnect();
                                           delay(1000);
                                           wifiBeginCalled = false;
                             }
                             if ((numOfClientsConnected == 0) && (WiFi.status() != WL_CONNECTED))
                             {
                                           // only once!!
                                           if (!wifiBeginCalled)
                                           {
                                                          WiFi.begin(wifiName.c_str(), wifiPass.c_str());
                                                          wifiBeginCalled = true;
                                           }
                             }
                             if ((numOfClientsConnected > 0) || (WiFi.status() == WL_CONNECTED))
                             {
                                           server.handleClient();
                                           if (wifiSetupPageVisited)
                                           {
                                                          if (server.args() >= 2)
                                                          { // Arguments were received
                                                                        String ssidName = server.arg(0);
                                                                        String ssidPassword = server.arg(1);

                                                                        Serial.println(server.args());
                                                                        Serial.println((const char*)ssidName.c_str());
                                                                        Serial.println((const char*)ssidPassword.c_str());
                                                                        wifiSetupPageVisited = false;
                                                                        if (spiffsOK)
                                                                        {
                                                                                      if (ssidName != wifiName || ssidPassword != wifiPass)
                                                                                      {
                                                                                                     file = SPIFFS.open("/wifiSetup.txt", "w");
                                                                                                     if (file)
                                                                                                     {
                                                                                                                   sprintf(tempStr, "%s\n", ssidName.c_str());
                                                                                                                   file.write((const unsigned char*)tempStr, strlen(tempStr));
                                                                                                                   sprintf(tempStr, "%s\n", ssidPassword.c_str());
                                                                                                                   file.write((const unsigned char*)tempStr, strlen(tempStr));

                                                                                                                   file.flush();
                                                                                                                   file.close();
                                                                                                                   wifiName = ssidName;
                                                                                                                   wifiPass = ssidPassword;
                                                                                                                   WiFi.disconnect();
                                                                                                                   delay(1000);
                                                                                                                   WiFi.begin(wifiName.c_str(), wifiPass.c_str());
                                                                                                     }
                                                                                      }
                                                                        }
                                                          }
                                           }
                             }
                             //lcd.fillScr(0, 0, 0);
                             //lcd.setColor(255, 255, 255);
                             if (WiFi.status() == WL_CONNECTED)
                             {
                                           //lcd.print("WiFi Connected", 0, 60, true);
                                           //lcd.print((char *)WiFi.localIP().toString().c_str(), 0, 100, true);
                             }
#if defined(MASTER)
                             size_t dataLength = 2;
                             uint8_t data[2] = { 0xAA, 0x55 };
#else
                             size_t dataLength = 3;
                             uint8_t data[3] = { 0x88, 0x33, 0x44 };
#endif

                             int iteration = 0;
                             uint8_t result;
              }
#endif
              while (1)
              {
                             ESP.wdtFeed();
                             if (dataRecv)
                             {
                                           printf("ESP-Now: Data Recived\n");
                                           printf("\n");
                                           dataRecv = false;
                                           String recData = data;
                                           printf("Data: %s\n", recData.c_str());
                                           if (!strcmp(recData.c_str(), "ping"))
                                           {
                                                          bool updateMAC = false;
                                                          for (size_t index = 0; index < MAC_LEN; index++)
                                                          {
                                                                        if (tempMACaddress[index] != masterMACaddress[index])
                                                                        {
                                                                                      updateMAC = true;
                                                                        }
                                                          }
                                                          if (spiffsOK && updateMAC)
                                                          {
                                                                        printf("Updating master's MAC \n");
                                                                        File file;
                                                                        char tempStr[20];
                                                                        sprintf(tempStr, "%02x:%02x:%02x:%02x:%02x:%02x", tempMACaddress[0], tempMACaddress[1], tempMACaddress[2], tempMACaddress[3], tempMACaddress[4], tempMACaddress[5]);
                                                                        String masterMAC = tempStr;
                                                                        printf("MAC: %s\n", masterMAC.c_str());
                                                                        file = SPIFFS.open("/master.txt", "w");
                                                                        if (file)
                                                                        {
                                                                                      file.write((const unsigned char *)masterMAC.c_str(), masterMAC.length());
                                                                                      file.close();
                                                                                      for (size_t i = 0; i < MAC_LEN; i++)
                                                                                      {
                                                                                                    masterMACaddress[i] = tempMACaddress[i];
                                                                                                     masterMacValid = true;
                                                                                      }
                                                                        }
                                                          }
                                           }
                                           if (!strcmp(recData.c_str(), "on"))
                                           {
                                                          powerOn = true;
                                                          digitalWrite(gpio13Led, HIGH);
                                                          digitalWrite(gpio12Relay, HIGH);
                                           }
                                           if (!strcmp(recData.c_str(), "off"))
                                           {
                                                          powerOn = false;
                                                          digitalWrite(gpio13Led, LOW);
                                                          digitalWrite(gpio12Relay, LOW);
                                           }
                                           if ((!strcmp(recData.c_str(), "getPowerState")) && masterMacValid)
                                           {
                                                          if (esp_now_add_peer(masterMACaddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0) != 0)
                                                                        printf("Failed to register peer\n");
                                                          else
                                                          {
                                                                        if (powerOn)
                                                                        {
                                                                                      espNowSendDataToPeer(masterMACaddress, "on", 10);
                                                                        }
                                                                        else
                                                                        {
                                                                                      espNowSendDataToPeer(masterMACaddress, "off", 10);
                                                                        }
                                                                        esp_now_del_peer(masterMACaddress);
                                                          }
                                           }
                             }
                             if (digitalRead(button) == LOW)
                             {
                                           while (digitalRead(button) == LOW) delay(10);
                                           powerOn = false;
                                           digitalWrite(gpio13Led, LOW);
                                           digitalWrite(gpio12Relay, LOW);
                             }
                             delay(10);
              }
}

// ESPnow related
// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
              //Serial.print("\r\nLast Packet Send Status:\t");
              //Serial.println(sendStatus == 0 ? "Deliverd Successfully" : "Delivery Failed");
              if (sendStatus == 0)
              {
                             dataSent = 1;
              }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
              dataRecv = true;
              size_t i;
              for (size_t j = 0; j < MAC_LEN; j++)
              {
                             tempMACaddress[j] = mac[j];
              }
              for (i = 0; i < len; i++)
              {
                             data[i] = incomingData[i];
              }
              data[i] = '\0';
}

bool espNowSendDataToPeer(uint8_t * macAddress, char * dataToSend, unsigned int retryCounter)
{
              if (strlen(dataToSend) >= ESP_NOW_MAX_DATA_LEN)
                             return false;
              for (size_t i = 0; i < retryCounter; i++)
              {
                             dataSent = 0;
                             if (esp_now_send(macAddress, (unsigned char*)dataToSend, strlen(dataToSend)) != 0)
                                           break;
                             delay(100);
                             if (dataSent == 1)
                                           return true;
              }
              return false;
}

bool extractMAC(String * rawMAC, uint8_t * MACarray)
{
              String tmpStr;
              unsigned int mac;
              char * ptr;

              // Verfiy rawMAC is OK
              if (rawMAC->length() < 17)
              {
                             printf("MAC len: %d\n", rawMAC->length());
                             return false;
              }
              for (size_t i = 0, index = 2; i < 6; i++, index += 3)
              {
                             if ((rawMAC->charAt(index) != ':') && i < 5)
                             {
                                           return false;
                             }
                             tmpStr = rawMAC->substring(index - 2, index);
                             MACarray[i] = strtol(tmpStr.c_str(), &ptr, 16); // If there other chars which are not hex number ptr will point to this srting thus its sine will be > 0
                             if (strlen(ptr) > 0)
                             {
                                           return false;
                             }
              }

              return true;
}

#ifdef ENABLE_WIFI
#define WIFI_TIMEOUT 20 // 10 seconds
bool connectToWiFiSTA(const char * name, const char * pass)
{
              unsigned int timeOutCounter = 0;
              WiFi.begin(name, pass);
              while ((WiFi.status() != WL_CONNECTED) && (timeOutCounter < WIFI_TIMEOUT))
              {
                             delay(500);
                             Serial.print(".");
                             timeOutCounter++;
              }
              if (timeOutCounter != WIFI_TIMEOUT)
              {
                             printf("\nWiFi connected.\n");
                             Serial.println(WiFi.localIP().toString().c_str());
                             Serial.println(WiFi.macAddress());
                             return true;
              }
              else
              {
                             printf("WiFi: cannot connect to: %s\n", name);
                             return false;
              }
              ESP.wdtFeed();
}

const char mainMenuPage[]PROGMEM = R"=====(
<!DOCTYPE html> 
<html>
<head><meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<title>ESP32 WebPage</title>
<style>
html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}
.button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 10px;}
.button-on {background-color: #1abc9c;}
.button-on:active {background-color: #16a085;}
p {font-size: 14px;color: #888;margin-bottom: 10px;}
</style>
</head>
<body>
<meta charset="utf-8">
<html lang="he">
<h1>SonOff lab web page</h1>
<a class="button button-on" href="/wifiSetupSelected">WiFi Setup</a>
</body>
<href="/">
</html>)=====";

const char setupWiFiHTMLpart1[]PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
<style>
html {font-family: Arial; display: inline-block; text-align: center;}
.button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 10px;}
.boxStyle {  padding: 12px 20px;  margin: 8px 0;  box-sizing: border-box;  border: 2px solid red;  border-radius: 10px; font-size: 20px;text-align: center;}
</style>
</head>
<body>
<form action="/" method="POST">
Access Point Name:<br>
<input type="text" class="boxStyle" name="AccessPoint" value=
)rawliteral";
//""><br>
const char setupWiFiHTMLpart2[]PROGMEM = R"rawliteral(
Password:<br>
<input type="text" class="boxStyle" name="Password" value=
)rawliteral";
//""><br>
const char setupWiFiHTMLpart3[]PROGMEM = R"rawliteral(
<input type="submit" class="button" value="OK">
</form>
</body>
<href="/">
</html>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 10px;}
  </style>
</head>
<body>
<h2>ESP Web Server</h2>
<h4>IO #1</h4>
<label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="1" checked><span class="slider"></span></label>
<h4>IO #2</h4>
<label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="2" checked><span class="slider"></span></label>
<h4>IO #3</h4>
<label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="3" checked><span class="slider"></span></label>
<h4><a class="button button-on" href="/">Home</a></h4>
<script>function toggleCheckbox(element) {
              var request = new XMLHttpRequest();
              var strOn            = "IO " + element.id + " ON";
              var strOff            = "IO " + element.id + " OFF";
              if(element.checked){ request.open("POST", "", true); request.setRequestHeader("Content-type", " ");request.send(strOn);}
              else { request.open("POST", "", true); request.setRequestHeader("Content-type", " ");request.send(strOff);}
              die();
}</script>
</body>
<href="/">
</html>
)rawliteral";

void handle_OnConnect()
{
              server.send(200, "text/html", mainMenuPage);
}

void wifiSetup_page()
{
              wifiSetupPageVisited = true;
              String setupWiFiHTML = setupWiFiHTMLpart1;
              //""><br>
              setupWiFiHTML += "\"";
              setupWiFiHTML += wifiName;
              setupWiFiHTML += "\"><br>";
              setupWiFiHTML += setupWiFiHTMLpart2;
              setupWiFiHTML += "\"";
              setupWiFiHTML += wifiPass;
              setupWiFiHTML += "\"><br>";
              setupWiFiHTML += setupWiFiHTMLpart3;
              setupWiFiHTML += "MAC Address: ";
              setupWiFiHTML += deviceMAC.c_str();
              setupWiFiHTML += "<br>";

              server.send(200, "text/html", setupWiFiHTML);
              printf("wifiSetupPageVisited\n");
}

void handle_NotFound()
{
              server.send(404, "text/plain", "Not found");
}
#endif
