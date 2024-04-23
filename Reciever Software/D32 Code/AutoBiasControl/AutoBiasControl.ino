#include <SPI.h>

#define CS_PIN 5
#define ADC_PIN 34
#define BUFFER 10

// Function to set potentiometer value
void setPotentiometerValue(byte value) {
  // Start SPI communication with MCP4151
  digitalWrite(CS_PIN, LOW);
  
  // Send command byte: 0b00010001 (Write to potentiometer, volatile wiper 0)
  SPI.transfer(0b00000000);
  
  // Send potentiometer value
  SPI.transfer(value);
  
  // End SPI communication
  digitalWrite(CS_PIN, HIGH);
}

void setup()
{
  SPI.begin();
  
  pinMode(CS_PIN, OUTPUT);
  
  Serial.begin(115200);
}

int avg = 0;
int pot = 25;

void loop()
{ 

  for (int i = 0; i < BUFFER; i++) {

    avg = avg + analogRead(ADC_PIN);
    delay(10);

  }

  avg = avg / (BUFFER);

  if (avg > 4000 && pot < 255) {

    pot++;

  }

  if (avg < 1000 && pot > 25) {

    pot--;

  }
  
  setPotentiometerValue(pot);

  Serial.print("pot:");
  Serial.print(pot);
  Serial.print(",");
  Serial.print("avg:");
  Serial.println(avg);

  avg = 0;

}
