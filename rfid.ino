#include <SPI.h>
#include <MFRC522.h>
#include "PCF8574.h"
#include <Wire.h>
#include "graphics.h"
#include "SPIFFS.h" 
PCF8574 pcf8574(0x21);
SPI_LCD_FrameBuffer lcd;

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
}

/**
 * Main loop.
 */
void loop() 
{
	char tmpStr[20];
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
		}
		// The receiving block needs regular retriggering (tell the tag it should transmit??)
		// (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
		activateRec(mfrc522);
		delay(100);
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
