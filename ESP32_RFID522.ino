{\rtf1\ansi\ansicpg1252\cocoartf2638
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 ArialMT;}
{\colortbl;\red255\green255\blue255;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c100000\c100000\c100000;\cssrgb\c0\c0\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs26\fsmilli13320 \cf0 \cb2 \expnd0\expndtw0\kerning0
/*
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0Name: ESP32_RFID522.ino
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0Created: 27-Mar-22 11:36:30 AM
\fs26\fsmilli13333 \

\fs26\fsmilli13320 */
\fs26\fsmilli13333 \

\fs26\fsmilli13320 /**
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 #include <SPI.h>
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #include <MFRC522.h>
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #include "PCF8574.h"
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #include <Wire.h>
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #include "graphics.h"
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #include "SPIFFS.h"\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 PCF8574 pcf8574(0x21);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 SPI_LCD_FrameBuffer lcd;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 class ttgoCDacc : public lcdHwAccessor
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 public:
\fs26\fsmilli13333 \

\fs26\fsmilli13320 ttgoCDacc() \{\};
\fs26\fsmilli13333 \

\fs26\fsmilli13320 ~ttgoCDacc() \{\};
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void setup()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 pinMode(5, OUTPUT); //chip select
\fs26\fsmilli13333 \

\fs26\fsmilli13320 pinMode(23, OUTPUT); //reset
\fs26\fsmilli13333 \

\fs26\fsmilli13320 pinMode(4, OUTPUT); //Back Light
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void reset()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 digitalWrite(23, LOW);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 delay(250);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 digitalWrite(23, HIGH);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 delay(250);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \};
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void assertCS()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 digitalWrite(5, LOW);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void deAssertCS()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 digitalWrite(5, HIGH);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void backLightOn()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 digitalWrite(4, HIGH);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void backLightOff()
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \} ttgoLCDaccessor;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 extern SPIClass MFRC522spi;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 #define RST_PIN\'a0 \'a0 \'a0 \'a0 \'a025\'a0 \'a0 \'a0 \'a0 \'a0 \'a0// Configurable, see typical pin layout above
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #define SS_PIN\'a0 \'a0 \'a0 \'a0 \'a0 12\'a0 \'a0 \'a0 \'a0 \'a0 \'a0// Configurable, see typical pin layout above
\fs26\fsmilli13333 \

\fs26\fsmilli13320 #define IRQ_PIN\'a0 \'a0 \'a0 \'a0 \'a027\'a0 \'a0 \'a0 \'a0 \'a0 \'a0// Configurable, depends on hardware
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 MFRC522 mfrc522(SS_PIN, RST_PIN);\'a0 \'a0// Create MFRC522 instance.
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 MFRC522::MIFARE_Key key;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 volatile bool bNewInt = false;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 byte regVal = 0x7F;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 void activateRec(MFRC522 mfrc522);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void clearInt(MFRC522 mfrc522);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void dump_byte_array(byte *buffer, byte bufferSize);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void readCard();
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /**
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* Initialize.
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void setup()\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.begin(115200); // Initialize serial communications with the PC
\fs26\fsmilli13333 \

\fs26\fsmilli13320 //while (!Serial);\'a0 \'a0 \'a0 // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 delay(250);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 pcf8574.begin();
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 if (!lcd.init(st7789_240x135x16_FB, &ttgoLCDaccessor, 16, 19, 18, 40000000))
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 printf("LCD init error\\n");
\fs26\fsmilli13333 \

\fs26\fsmilli13320 while (1);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 MFRC522spi.begin(15, 36, 13, -1);\'a0 \'a0 \'a0 \'a0 \'a0 // Init SPI bus
\fs26\fsmilli13333 \

\fs26\fsmilli13320 MFRC522spi.setFrequency(4000000);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 mfrc522.PCD_Init(); // Init MFRC522 card
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.print(F("Ver: 0x"));
\fs26\fsmilli13333 \

\fs26\fsmilli13320 byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.println(readReg, HEX);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /* setup the IRQ pin*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 pinMode(IRQ_PIN, INPUT_PULLUP);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /*
\fs26\fsmilli13333 \

\fs26\fsmilli13320 * Allow the ... irq to be propagated to the IRQ pin
\fs26\fsmilli13333 \

\fs26\fsmilli13320 * For test purposes propagate the IdleIrq and loAlert
\fs26\fsmilli13333 \

\fs26\fsmilli13320 */
\fs26\fsmilli13333 \

\fs26\fsmilli13320 regVal = 0xA0; //rx irq
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PCD_WriteRegister(mfrc522.ComIEnReg, regVal);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 bNewInt = false; //interrupt flag
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /*Activate the interrupt*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 attachInterrupt(digitalPinToInterrupt(IRQ_PIN), readCard, FALLING);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /*do\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{ //clear a spourious interrupt at start
\fs26\fsmilli13333 \

\fs26\fsmilli13320 ;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \} while (!bNewInt);*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 bNewInt = false;
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 Serial.println(F("End setup"));
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /**
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* Main loop.
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void loop()\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 char tmpStr[20];
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.loadFonts(ORBITRON_LIGHT24);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.setColor(255, 0, 0);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 int i,j;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 unsigned int ID;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.fillScr(255, 255, 255);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.print("Pass your card", 0, 135 / 2 - 24 / 2, true);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.flushFrameBuffer();
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 while (1)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 if (bNewInt)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{ //new read interrupt
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.print(F("Interrupt. "));
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PICC_ReadCardSerial(); //read the tag data
\fs26\fsmilli13333 \

\fs26\fsmilli13320 // Show some details of the PICC (that is: the tag/card)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.print(F("Card UID:"));
\fs26\fsmilli13333 \

\fs26\fsmilli13320 dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.println();
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.fillScr(255, 255, 255);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 j = 0;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 for (i = 0; i < mfrc522.uid.size; i++)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 sprintf(&tmpStr[j], "%02X", mfrc522.uid.uidByte[i]);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 j += 2;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 tmpStr[j] = '\\0';
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.print(tmpStr, 0, 135 / 2 - 24 / 2, true);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 lcd.flushFrameBuffer();
\fs26\fsmilli13333 \

\fs26\fsmilli13320 ID = strtol(tmpStr, (char **)NULL, 16);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 printf("Str: %s Card ID: %08X\\n", tmpStr, ID);
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 clearInt(mfrc522);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PICC_HaltA();
\fs26\fsmilli13333 \

\fs26\fsmilli13320 bNewInt = false;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 // The receiving block needs regular retriggering (tell the tag it should transmit??)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 // (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
\fs26\fsmilli13333 \

\fs26\fsmilli13320 activateRec(mfrc522);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 delay(100);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \} //loop()
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /**
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* Helper routine to dump a byte array as hex values to Serial.
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void dump_byte_array(byte *buffer, byte bufferSize)\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 for (byte i = 0; i < bufferSize; i++)\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.print(buffer[i] < 0x10 ? " 0" : " ");
\fs26\fsmilli13333 \

\fs26\fsmilli13320 Serial.print(buffer[i], HEX);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \

\fs26\fsmilli13320 /**
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* MFRC522 interrupt serving routine
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void readCard()\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 bNewInt = true;
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /*
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* The function sending to the MFRC522 the needed commands to activate the reception
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void activateRec(MFRC522 mfrc522)\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_REQA);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PCD_WriteRegister(mfrc522.CommandReg, mfrc522.PCD_Transceive);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \
\pard\pardeftab720\partightenfactor0

\fs26\fsmilli13320 \cf0 \cb1 \
\pard\pardeftab720\partightenfactor0
\cf0 \cb2 /*
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0* The function to clear the pending interrupt bits after interrupt serving routine
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \'a0*/
\fs26\fsmilli13333 \

\fs26\fsmilli13320 void clearInt(MFRC522 mfrc522)\'a0
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \{
\fs26\fsmilli13333 \

\fs26\fsmilli13320 mfrc522.PCD_WriteRegister(mfrc522.ComIrqReg, 0x7F);
\fs26\fsmilli13333 \

\fs26\fsmilli13320 \}
\fs26\fsmilli13333 \
}