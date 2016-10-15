/* Connections:                                           */
/* 3V3 & GND                                              */
/* SIO_C  - 19/A5/SCL0, 4.7kOhm pull-up resistor required */
/* SIO_D  - 18/A4/SDA0, 4.7kOhm pull-up resistor required */
/* XCLK   - 3/TX/PWM                                      */

#include <Wire.h>

const byte SIO_C  = 19;
const byte SIO_D  = 18;
const byte XCLK   = 3;

void setupSystemClockOutput()
{
  const float clockFrequencyInHz = 8000000;
  const int maxValue = 255;
  analogWriteFrequency(XCLK, clockFrequencyInHz);
  analogWrite(XCLK, maxValue / 2);
}

void scanForAvailableSensors()
{
  for (int address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("Sensor found at address: %i.\n", address);
    }
  }
}

void setup() {
  setupSystemClockOutput();
  Wire.begin();
}

void loop() {
  scanForAvailableSensors();
  delay(5000);
}
