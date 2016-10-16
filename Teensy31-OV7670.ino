/* Connections:                                           */
/* 3V3 & GND                                              */
/* SIO_C  - 19/A5/SCL0, 4.7kOhm pull-up resistor required */
/* SIO_D  - 18/A4/SDA0, 4.7kOhm pull-up resistor required */
/* XCLK   - 3/TX/PWM                                      */

#include <Wire.h>

const byte SIO_C  = 19;
const byte SIO_D  = 18;
const byte XCLK   = 3;

struct RevisterValue {
  byte registerAddress;
  byte registerData;
};

// TODO: registers copied from another project, evaluate if necessary
const struct RevisterValue OV7670Registers[] = {
  {0x12, 0x80}, // Reset to default values
  {0x11, 0x80},
  {0x3B, 0x0A},
  {0x3A, 0x04},
  {0x3A, 0x04},
  {0x12, 0x04}, // Output format: rgb
  {0x8C, 0x00}, // Disable RGB444
  {0x40, 0xD0}, // Set RGB565
  {0x17, 0x16},
  {0x18, 0x04},
  {0x32, 0x24},
  {0x19, 0x02},
  {0x1A, 0x7A},
  {0x03, 0x0A},
  {0x15, 0x02},
  {0x0C, 0x04},
  {0x3E, 0x1A}, // Divide by 4
  {0x1E, 0x27},
  {0x72, 0x22}, // Downsample by 4
  {0x73, 0xF2}, // Divide by 4
  {0x4F, 0x80},
  {0x50, 0x80},
  {0x51, 0x00},
  {0x52, 0x22},
  {0x53, 0x5E},
  {0x54, 0x80},
  {0x56, 0x40},
  {0x58, 0x9E},
  {0x59, 0x88},
  {0x5A, 0x88},
  {0x5B, 0x44},
  {0x5C, 0x67},
  {0x5D, 0x49},
  {0x5E, 0x0E},
  {0x69, 0x00},
  {0x6A, 0x40},
  {0x6B, 0x0A},
  {0x6C, 0x0A},
  {0x6D, 0x55},
  {0x6E, 0x11},
  {0x6F, 0x9F},
  {0xB0, 0x84},
  {0xFF, 0xFF}  // End marker
};

void setupSystemClockOutput()
{
  const float clockFrequencyInHz = 8000000;
  const int maxValue = 255;
  analogWriteFrequency(XCLK, clockFrequencyInHz);
  analogWrite(XCLK, maxValue / 2);
}

byte getFirstAvailableSensorAddress()
{
  int error = -1;
  byte address = 1;
  
  for (; address <= 127 && error != 0; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
  }

  Serial.printf("Sensor found at: %i.\n", --address);

  return address;
}

int writeRegister(
  byte deviceAddress,
  byte registerAddress,
  byte registerData) {
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(registerData);
    return Wire.endTransmission();
}

int writeRegisterArray(
  byte deviceAddress,
  const struct RevisterValue *registerValues) {        
    while (registerValues -> registerAddress != 0xff || registerValues -> registerData != 0xff) {
      int transmissionResult = writeRegister(deviceAddress, registerValues -> registerAddress, registerValues -> registerData);
      Serial.printf("Register write status: %i.\n", transmissionResult);

      if (transmissionResult < 0) {  // TODO: not sure if this can ever be < 0
        return transmissionResult;
      }
      
      registerValues++;
    }
    
    return 0; // TODO
}

void initOV7670(byte deviceAddress) {
  writeRegisterArray(deviceAddress, OV7670Registers);
}

void setup() {
  setupSystemClockOutput();
  Wire.begin();
}

void loop() {
  byte sensorAddress = getFirstAvailableSensorAddress();
  initOV7670(sensorAddress);
  
  delay(5000);
}
