#include <SPI.h>

const int CS_PIN = 10;  // Chip Select (CS) pin

/*
// 37.1 kHz
const PROGMEM uint16_t bit_zero[21] =
{
 511,  661,  798,  910,  986, 1020, 1009,  953, 
 858,  732,  587,  434,  289,  163,   68,   12, 
   1,   35,  111,  223,  360
};
*/

/*
// 95.7 kHz
const PROGMEM uint16_t bit_zero[8] =
{
390,  665,  780,  665,  390,  114,    0,  114
};
*/



const PROGMEM uint16_t bit_zero[8] =
{
260,  443,  520,  443,  260,   76,    0,   76
};

const PROGMEM uint16_t bit_one[16] = 
{
511,  872, 1022,  872,  511,  149,    0,  149, 
 510,  872, 1022,  872,  511,  149,    0,  149
};

SPISettings settings(20000000, MSBFIRST, SPI_MODE0);

void sendDACValue(unsigned int value) {
  // Construct the DAC value
  uint16_t message = 0x1000 | (value << 2);

  // Activate the SPI communication
  SPI.beginTransaction(settings);
  digitalWrite(CS_PIN, LOW);
  
  // Send message byte
  SPI.transfer16(message);
  
  // Deactivate SPI communication
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
}

void setup() {
  // Initialize SPI communication
  SPI.begin();
  // Set CS pin as output
  pinMode(CS_PIN, OUTPUT);
}

char data = 0xA3;

void loop() {

  for (int i = 0; i < 8; i++) {sendDACValue (bit_zero[i]);}
  //for (int i = 0; i < 16; i++) {sendDACValue (bit_one[i]);}

  // Modulator
  /*for (int i = 7; i >= 0; i--) {

    unsigned char bit = (data >> i) & 1;

    if (bit == 1) {

      for (int i = 0; i < 128; i++) {sendDACValue (bit_one[i]);}

    } else {

      for (int i = 0; i < 128; i++) {sendDACValue (bit_zero[i]);}

    }

  }*/

}



