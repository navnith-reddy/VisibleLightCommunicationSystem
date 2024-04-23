#include <SPI.h>

// 35 kHz bitmap
const PROGMEM uint16_t bit_zero[22] =
{
 390,  499,  600,  684,  744,  776,  776,  744, 
 684,  600,  499,  390,  280,  179,   95,   35, 
   3,    3,   35,   95,  179,  280
};

// 45 kHz bitmap
const PROGMEM uint16_t bit_one[17] = 
{
 390,  530,  652,  739,  778,  765,  701,  595, 
 461,  318,  184,   78,   14,    1,   40,  127, 
 249
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

byte encoder(uint8_t nibble){
  switch(nibble) {
    case 0:
      return 0b00000000;
      break;
    case 1:
      return 0b00010011;
      break;
    case 2:
      return 0b00100100;
      break;
    case 3:
      return 0b00110111;
      break;
    case 4:
      return 0b01000001;
      break;
    case 5:
      return 0b01010010;
      break;
    case 6:
      return 0b01100101;
      break;
    case 7:
      return 0b01110110;
      break;
    case 8:
      return 0b10001101;
      break;
    case 9:
      return 0b10011110;
      break;
    case 10:
      return 0b10101001;
      break;
    case 11:
      return 0b10111010;
      break;
    case 12:
      return 0b11001100;
      break;
    case 13:
      return 0b11011111;
      break;
    case 14:
      return 0b11101000;
      break;
    case 15:
      return 0b11111011;
      break;
    default:
      return 0b11111111;//Error
      break;
  }
}

word encodeByte (uint8_t byte){

  // Bitmask first nibble and encode
  firstByte = encoder((byte & 0xF0) >> 4);

  // Bitmask last nibble and encode
  lastByte = encoder(byte & 0x0F);

  // Return encoded word
  return ((firstByte << 8) | (lastByte));
}

char testByte = 0xA3;

void loop() {

  // Encode test data
  word data = encodeByte(testByte);
  
  for (int i = 16; i >= 0; i--) {

    unsigned char bit = (data >> i) & 1;

    if (bit == 1) {

      for (int i = 0; i < 128; i++) {sendDACValue (bit_one[i]);}

    } else {

      for (int i = 0; i < 128; i++) {sendDACValue (bit_zero[i]);}

    }

  }

}



