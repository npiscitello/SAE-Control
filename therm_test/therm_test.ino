#include <Wire.h>
#include "SparkFunMLX90614.h"

// default = 0x5A
const uint8_t THERM_SLAVE_ADDR = 0x5A;
const uint8_t THERM_READ_TA_CMD = 0x06;
const uint8_t THERM_REPLY_BYTES = 3;

float temp_reading = 0;

IRTherm temp;

void setup() {
  temp.begin(THERM_SLAVE_ADDR);
  temp.setUnit(TEMP_F);
  Serial.begin(115200);
}

void loop() {
  
  if( temp.read() ) {
    temp_reading = temp.object();
    Serial.println("temp: " + String(temp_reading));
  }

  delay(100);
}
