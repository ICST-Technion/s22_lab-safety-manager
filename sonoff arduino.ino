
#include "FS.h"

bool spiffsOK = false;

/* ESP Now Related*/
#include <ESP8266WiFi.h>
#include "espnow.h"
// declaration of callback functions
void OnDataSent(const uint8_t* mac_addr, uint8_t status);
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

#define PEER

// MAC addresses
#define MAC_LEN 6
uint8_t masterMACaddress[] = { 0x10, 0x52, 0x1C, 0x66, 0x74, 0x60 }; // Main board
uint8_t peer01MACaddress[] = { 0x24, 0x62, 0xAB, 0xF9, 0x96, 0x20 }; // dongle board - 24:62:AB:F1:A5:24
uint8_t* targetMACaddress;
String deviceMAC;

#ifdef ENABLE_WIFI
// WiFi part
//#include <WiFi.h>
//#include <ESPmDNS.h>
//#include <WebServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
bool wifiSetupPageVisited = false;
#endif

// the setup function runs once when you press reset or power the board
void setup()
{
    // Setup the IOs
    Serial.begin(115200);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.channel(0);
    Serial.print("debug1");

    delay(500);

    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(12, LOW); //red off
    digitalWrite(13, LOW); //blue on
    //pinMode(13, OUTPUT);
    //digitalWrite(13, LOW);
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

    /* Callback Functions */
    // On Tx
    //esp_now_register_send_cb(OnDataSent);
    // On Rx
    //esp_now_register_recv_cb(OnDataRecv);

    // Register peer
    //if (esp_now_add_peer(peer01MACaddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0) != 0)
    //           printf("Failed to register peer\n");
}

bool dataRecv = false;
unsigned char data[256];

void loop()
{
    int numOfClientsConnected;
    //lcd.loadFonts(ORBITRON_LIGHT24);
    File file;
    char tempStr[32];
    bool IO3set = true;
    unsigned int counter = 0;

    while (1)
    {
        // Send message via ESP-NOW
        // result = esp_now_send(targetMACaddress, data, dataLength);
        //delay(2000);
        ESP.wdtFeed();
        if (dataRecv)
        {
            dataRecv = false;
            printf("Data: %s\n", data);
            sscanf((const char*)data, "%d", &counter);
            counter = counter * 600; //num of sec in minute, in milli sec 
        }
        if (counter > 0)
        {
            counter--;
            digitalWrite(12, HIGH); //red on
            digitalWrite(13, HIGH); //blue off

        }
        else
        {
            digitalWrite(12, LOW); //red off
            digitalWrite(13, LOW); //blue on
        }
        delay(100);
       
        //printf(".");
    }
}

// ESPnow related
// Callback when data is sent
void OnDataSent(uint8_t* mac_addr, uint8_t sendStatus)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(sendStatus == 0 ? "Deliverd Successfully" : "Delivery Failed");
}

// Callback when data is received
void OnDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len)
{
    dataRecv = true;
    size_t i;
    for (i = 0; i < len; i++)
    {
        data[i] = incomingData[i];
    }
    data[i] = '\0';
}

