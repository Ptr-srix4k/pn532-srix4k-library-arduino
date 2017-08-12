# pn532-srix4k-library-arduino

Modified Adafruit PN532 library for reading/writing SRIX4K tags 

Support basic commands for read/write:
- bool srix4k_init(void);
- bool srix4k_initiate_select(void);
- bool srix4k_readblock(uint8_t address, uint8_t *block);
- bool srix4k_writeblock(uint8_t address, uint8_t block[4]);
- bool srix4k_uid(uint8_t *block);

To use the library follow these steps

1. Install Adafruit libraries for PN532 from https://github.com/adafruit/Adafruit-PN532
2. Copy the two files "Adafruit_PN532.c++" and "Adafruit_PN532.h" inside the Adafruit arduino libray folder    
   for example : Arduino\libraries\Adafruit_PN532  
3. Use the sketch called "SRIX4k" to test the library
