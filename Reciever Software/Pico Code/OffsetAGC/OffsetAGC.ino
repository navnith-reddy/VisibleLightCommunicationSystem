#include <SPI.h>
#include <ADCInput.h>

#define CS_PIN0 17
#define ADC_PIN 34
#define BUFFER 5000
#define OFFSET 2048
#define REF 1024

SPISettings settings(10000000, MSBFIRST, SPI_MODE0);

// Function to set potentiometer value
void setPotVal(byte value) {
  // Start SPI communication with MCP4151
  SPI.beginTransaction(settings);
  digitalWrite(CS_PIN0, LOW);
  
  // Send command byte: 0b00010001 (Write to potentiometer, volatile wiper 0)
  SPI.transfer(0x0);
  
  // Send potentiometer value
  SPI.transfer(value);
  
  digitalWrite(CS_PIN0, HIGH);
  // End SPI communication
  SPI.endTransaction();
}

// -----------------------------------------------------------
// ------------------------ CORE ZERO ------------------------
// -----------------------------------------------------------

ADCInput adc0(A0);

void setup()
{
  SPI.begin();
  pinMode(CS_PIN0, OUTPUT);

  // Initialise with unity gain
  setPotVal(1);

  Serial.begin(115200);

  // Start ADC at 10ksps
  adc0.begin(500000);
}

int avg = 0;
int i = 0;
int pot = 0;

void loop()
{ 
  if (adc0.available()) {

    int16_t sample = adc0.read();

    sample = abs(sample - OFFSET);

    avg += sample;
    
    i++;

    if (i == BUFFER) {
      
      
      // Calculate average and reset counter
      avg = avg / BUFFER;
      i = 0;
      Serial.printf("avg:%d\n", avg);
      Serial.printf("pot:%d\n", (pot << 2));

      // Reduce gain if DC component is greater than reference
      if ((avg - REF) > 0) {

        pot--;
        if (pot < 0) {

          pot = 0;

        }
        setPotVal(pot);

      }

      // Increase gain if DC component is less than reference
      if ((avg - REF) < 0) {

          
        pot++;
        if (pot > 255) {

          pot = 255;

        }
        setPotVal(pot);

      }

      // Reset average
      avg = 0;

    }

  }

}

