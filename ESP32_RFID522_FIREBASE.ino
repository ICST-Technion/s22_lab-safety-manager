/*
 Name:		ESP32_RFID522_FIREBASE.ino
 Created:	27-Mar-22 11:36:30 AM
*/
/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 *
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 *
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * Minimal example how to use the interrupts to read the UID of a MIFARE Classic PICC
 * (= card/tag).
 *
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        3                10
 * IRQ         ?            ?             ?         ?          2                10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

// for firebase:
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "The Promised LAN"
#define WIFI_PASSWORD "SissoCatanMe0"
/* 2. Define the API Key */
#define API_KEY "AIzaSyDVqDgltLIhBE6_KHj6jp337cB84uQmmbM"
/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "safetymanager-52fc3"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "iot.project.tns@gmail.com"
#define USER_PASSWORD "iot12345"



// for RFID
#include <SPI.h>
#include <MFRC522.h>
#include "PCF8574.h"
#include <Wire.h>
#include "graphics.h"
#include "SPIFFS.h" 
#include <string>
PCF8574 pcf8574(0x21);
SPI_LCD_FrameBuffer lcd;

// for firebase:
// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;
unsigned long dataMillis = 0;


class ttgoCDacc : public lcdHwAccessor
{
public:
	ttgoCDacc() {};
	~ttgoCDacc() {};
	void setup()
	{
		pinMode(5, OUTPUT); //chip select
		pinMode(23, OUTPUT); //reset
		pinMode(4, OUTPUT); //Back Light
	}
	void reset()
	{
		digitalWrite(23, LOW);
		delay(250);
		digitalWrite(23, HIGH);
		delay(250);
	};
	void assertCS()
	{
		digitalWrite(5, LOW);
	}
	void deAssertCS()
	{
		digitalWrite(5, HIGH);
	}
	void backLightOn()
	{
		digitalWrite(4, HIGH);
	}
	void backLightOff()
	{

	}
} ttgoLCDaccessor;

extern SPIClass MFRC522spi;

#define RST_PIN         25           // Configurable, see typical pin layout above
#define SS_PIN          12           // Configurable, see typical pin layout above
#define IRQ_PIN         27           // Configurable, depends on hardware

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

volatile bool bNewInt = false;
byte regVal = 0x7F;

void activateRec(MFRC522 mfrc522);
void clearInt(MFRC522 mfrc522);
void dump_byte_array(byte *buffer, byte bufferSize);
void readCard();

/**
 * Initialize.
 */
void setup() 
{
	Serial.begin(115200); // Initialize serial communications with the PC
	//while (!Serial);      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	delay(250);

	pcf8574.begin();

	if (!lcd.init(st7789_240x135x16_FB, &ttgoLCDaccessor, 16, 19, 18, 40000000))
	{
		printf("LCD init error\n");
		while (1);
	}

	MFRC522spi.begin(15, 36, 13, -1);          // Init SPI bus
	MFRC522spi.setFrequency(4000000);

	mfrc522.PCD_Init(); // Init MFRC522 card

	/* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
	Serial.print(F("Ver: 0x"));
	byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
	Serial.println(readReg, HEX);

	/* setup the IRQ pin*/
	pinMode(IRQ_PIN, INPUT_PULLUP);

	/*
	 * Allow the ... irq to be propagated to the IRQ pin
	 * For test purposes propagate the IdleIrq and loAlert
	 */
	regVal = 0xA0; //rx irq
	mfrc522.PCD_WriteRegister(mfrc522.ComIEnReg, regVal);

	bNewInt = false; //interrupt flag

	/*Activate the interrupt*/
	attachInterrupt(digitalPinToInterrupt(IRQ_PIN), readCard, FALLING);

	/*do 
	{ //clear a spourious interrupt at start
		;
	} while (!bNewInt);*/
	bNewInt = false;

	Serial.println(F("End setup"));

  // for firebase:
    //  Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
}

/**
 * Main loop.
 */
void loop() 
{
  bool card = false;
	char tmpStr[14];
	lcd.loadFonts(ORBITRON_LIGHT24);
	lcd.setColor(255, 0, 0);
	int i,j;
	unsigned int ID;
	lcd.fillScr(255, 255, 255);
	lcd.print("Pass your card", 0, 135 / 2 - 24 / 2, true);
	lcd.flushFrameBuffer();
  
	while (1)
	{
		if (bNewInt)
		{ //new read interrupt
			Serial.print(F("Interrupt. "));
			mfrc522.PICC_ReadCardSerial(); //read the tag data
			// Show some details of the PICC (that is: the tag/card)
			Serial.print(F("Card UID:"));
			dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
			Serial.println();
			lcd.fillScr(255, 255, 255);
			j = 0;
			for (i = 0; i < mfrc522.uid.size; i++)
			{
				sprintf(&tmpStr[j], "%02X", mfrc522.uid.uidByte[i]);
				j += 2;
			}
			tmpStr[j] = '\0';
			lcd.print(tmpStr, 0, 135 / 2 - 24 / 2, true);
			lcd.flushFrameBuffer();
			ID = strtol(tmpStr, (char **)NULL, 16);
			printf("Str: %s Card ID: %08X\n", tmpStr, ID);

			clearInt(mfrc522);
			mfrc522.PICC_HaltA();
			bNewInt = false;
      card = true;
		}
		// The receiving block needs regular retriggering (tell the tag it should transmit??)
		// (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
		activateRec(mfrc522);
		delay(100);
    
    // for firebase:
    if (card && Firebase.ready() && (millis() - dataMillis > 60000 || dataMillis == 0))
    {
      dataMillis = millis();
      if (!taskCompleted)
      {
        taskCompleted = true;
        // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
        FirebaseJson content;
        // info is the collection id, countries is the document id in collection info.
        char buffer[30];
        char a[] = "Registration/";
        strcpy(buffer, a);
        strcat(buffer, tmpStr);
        //String documentPath = ("Registration/" + String(tmpStr));

        content.set("fields/time/stringValue", "put time stamp tommorow :) ");

        Serial.print("Create document... ");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, buffer, content.raw()))
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
          Serial.println(fbdo.errorReason());
      }

      String documentPath = "Registration/" ;
      String mask = "Singapore";

      // If the document path contains space e.g. "a b c/d e f"
      // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

      Serial.print("Get a document... ");

      if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
          Serial.println(fbdo.errorReason());
    }
    taskCompleted = false;
    card = false;
  }
} //loop()

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) 
{
	for (byte i = 0; i < bufferSize; i++) 
	{
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
}
/**
 * MFRC522 interrupt serving routine
 */
void readCard() 
{
	bNewInt = true;
}

/*
 * The function sending to the MFRC522 the needed commands to activate the reception
 */
void activateRec(MFRC522 mfrc522) 
{
	mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_REQA);
	mfrc522.PCD_WriteRegister(mfrc522.CommandReg, mfrc522.PCD_Transceive);
	mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);
}

/*
 * The function to clear the pending interrupt bits after interrupt serving routine
 */
void clearInt(MFRC522 mfrc522) 
{
	mfrc522.PCD_WriteRegister(mfrc522.ComIrqReg, 0x7F);
}
