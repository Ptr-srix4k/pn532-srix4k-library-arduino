/**************************************************************************/
/*! 
    @file     SRIX4k.pde
    @author   Ptr

    This example will attempt to connect to an SRIX4K
    card or tag and retrieve some basic information about it.
    It is based on the iso14443a_uid scketch from Adafruit Industries
    
    Note that you need the baud rate to be 115200 because we need to print
	out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
*/
/**************************************************************************/
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <SPI.h>


// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a SPI connection:
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

void setup(void) {
  boolean success;
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  success = nfc.srix4k_init();
  if (!success)
  {
    Serial.println("InlistPassiveTarget command error");
    while (1); // halt
  }
  
  Serial.println("Waiting for an SRIX4K card");
}

void loop(void) {
  boolean success;
  uint8_t data;
  uint8_t block[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
  uint8_t writeblock[] = { 0xFF, 0xFF, 0xFF, 0xFF };

  success = nfc.srix4k_initiate_select();
  if (!success)
  {
    Serial.println("Failed to INITIATE and SELECT the SRIX4K tag");
    while (1); // halt
  }
  
  Serial.println("Found a card!");

  //Reading block 0x10
  success = nfc.srix4k_readblock(0x10, block);  
  if (!success)
  {
    Serial.println("Failed to read block 0x10");
    while (1); // halt
  }
  Serial.print("0x10 : ");
  Serial.print(block[0], HEX); Serial.print(" ");
  Serial.print(block[1], HEX); Serial.print(" ");
  Serial.print(block[2], HEX); Serial.print(" ");
  Serial.print(block[3], HEX); Serial.print(" "); 
  Serial.println(""); 
  //-----------------------------------------------  

  //Reading block 0x7F
  success = nfc.srix4k_readblock(0x7F, block);  
  if (!success)
  {
    Serial.println("Failed to read block 0x7F");
    while (1); // halt
  }
  Serial.print("0x7F : ");
  Serial.print(block[0], HEX); Serial.print(" ");
  Serial.print(block[1], HEX); Serial.print(" ");
  Serial.print(block[2], HEX); Serial.print(" ");
  Serial.print(block[3], HEX); Serial.print(" "); 
  Serial.println(""); 
  //----------------------------------------------- 
/*
  //Writing block 0x7F
  success = nfc.srix4k_writeblock(0x7F, writeblock);  
  if (!success)
  {
    Serial.println("Failed to write block 0x7F");
    while (1); // halt
  }
  //----------------------------------------------- 

  //Reading block 0x7F
  success = nfc.srix4k_readblock(0x7F, block);  
  if (!success)
  {
    Serial.println("Failed to read block 0x7F");
    while (1); // halt
  }
  Serial.print("0x7F : ");
  Serial.print(block[0], HEX); Serial.print(" ");
  Serial.print(block[1], HEX); Serial.print(" ");
  Serial.print(block[2], HEX); Serial.print(" ");
  Serial.print(block[3], HEX); Serial.print(" "); 
  Serial.println(""); 
  //----------------------------------------------- 
*/
  //Reding UID 
  success = nfc.srix4k_uid(block);  
  if (!success)
  {
    Serial.println("Failed to read UID");
    while (1); // halt
  }
  Serial.print("UID : ");
  Serial.print(block[0], HEX); Serial.print(" ");
  Serial.print(block[1], HEX); Serial.print(" ");
  Serial.print(block[2], HEX); Serial.print(" ");
  Serial.print(block[3], HEX); Serial.print(" "); 
  Serial.print(block[4], HEX); Serial.print(" "); 
  Serial.print(block[5], HEX); Serial.print(" "); 
  Serial.print(block[6], HEX); Serial.print(" "); 
  Serial.print(block[7], HEX); Serial.print(" "); 
  Serial.println(""); 
  //----------------------------------------------- 

  while (1); // halt

}
