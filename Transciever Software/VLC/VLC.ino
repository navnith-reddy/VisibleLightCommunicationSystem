#include <SPI.h>
#include <ADCInput.h>

#define CS_PIN 5
#define CS_PIN0 17
#define ADC_PIN 34
#define BUFFER 64

SPISettings settings(10000000, MSBFIRST, SPI_MODE0);

// Function to set potentiometer value
void setPotVal(byte value) {
  // Start SPI communication with MCP4151
  SPI1.beginTransaction(settings);
  
  // Send command byte: 0b00010001 (Write to potentiometer, volatile wiper 0)
  SPI1.transfer(0x0);
  
  // Send potentiometer value
  SPI1.transfer(value);
  
  // End SPI communication
  SPI1.endTransaction();
}

SPISettings settings0(20000000, MSBFIRST, SPI_MODE0);

void sendDACValue(unsigned int value) {
  // Construct the DAC value
  uint16_t message = 0x1000 | (value << 2);

  // Activate the SPI communication
  SPI.beginTransaction(settings0);
  digitalWrite(CS_PIN0, LOW);
  
  // Send message byte
  SPI.transfer16(message);
  
  // Deactivate SPI communication
  digitalWrite(CS_PIN0, HIGH);
  SPI.endTransaction();
}

// 76.7 kHz
const PROGMEM uint16_t bit_one[10] =
{
 300,  476,  585,  585,  476,  300,  123,   14, 
  14,  123
};

void setup1()
{
  // Initialize SPI communication
  SPI.setTX(19);
  SPI.setSCK(18);
  SPI.setCS(17);
  SPI.begin();
  // Set CS pin as output
  pinMode(CS_PIN0, OUTPUT);
}

void loop1()
{
  //for (int i = 0; i < 10; i++) {sendDACValue (bit_one[i]);}
  sendDACValue(600);
  sendDACValue(0);
}

ADCInput adc(A0);

void setup()
{
  SPI1.begin();

  // Initialise with unity gain
  setPotVal(1);

  Serial.begin(115200);

  // Start ADC at 10ksps
  adc.begin(20000);
}

int avg = 0;
int i = 0;
int pot = 0;
int AGCref = 2048;

void loop()
{ 
  
  if (adc.available()) {

    int16_t sample = adc.read();

    avg += sample;
    
    i++;

    if (i == BUFFER) {
      
      
      // Calculate average and reset counter
      avg = avg / BUFFER;
      i = 0;
      Serial.printf("avg:%d\n", avg);

      // Reduce gain if DC component is greater than reference
      if ((avg - AGCref) > 0) {

        pot--;
        setPotVal(pot);

      }

      // Increase gain if DC component is less than reference
      if ((avg - AGCref) < 0) {

          
        pot++;
        setPotVal(pot);

      }

      // Reset average
      avg = 0;

    }

  }

}
