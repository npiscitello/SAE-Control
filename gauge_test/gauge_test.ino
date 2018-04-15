// this is the inbuilt Arduino IDE SPI lib
#include <SPI.h>

#define DEBUG

#define SS_PIN 10

// MAX 500000
#define SPI_SCK_SPEED 250000
#define SPI_BIT_ORDER MSBFIRST
// CPOL = 0, CPHA = 0, Data out on falling, capture on rising
// See https://www.arduino.cc/en/Reference/SPI
#define SPI_MODE SPI_MODE0

enum OPCODE {
  LOGO1 = 0x01,
  LOGO2 = 0x03,
  LOGO3 = 0x04,
  LOGO4 = 0x06,
  LOGO5 = 0x05
};

enum QUADRANT {
  Q1 = 0x00,
  Q2 = 0x01,
  Q3 = 0x02,
  Q4 = 0x03
};

void write_gauges(OPCODE gauge, uint8_t angle, QUADRANT quadrant);



void setup() {

  pinMode(SS_PIN, OUTPUT);
  SPI.begin();

#ifdef DEBUG
  Serial.begin(115200);
  write_gauges(LOGO2, 126, Q3);
#endif
}



void loop() {
  int i = 1;
  for( i; i < 90; i++ ) {
    write_gauges(LOGO2, i, Q1);
    delay(10);
  }
  for( i; i > 0; i-- ) {
    write_gauges(LOGO2, i, Q1);
    delay(10);
  }
}



void write_gauges(OPCODE gauge, uint8_t angle, QUADRANT quadrant) {
  // construct data packet
  // start bit
  uint16_t data = 0x8000;

  // opcode
  data |= ((uint16_t)(gauge & 0x0003) << 12);

  // angle
  data |= ((uint16_t)(angle & 0x00FF) << 2);

  // quadrant
  data |= (quadrant & 0x03);

  SPI.beginTransaction(SPISettings(SPI_SCK_SPEED, SPI_BIT_ORDER, SPI_MODE));
  digitalWrite(SS_PIN, HIGH);
  SPI.transfer(data);
  digitalWrite(SS_PIN, LOW);
  SPI.endTransaction();

#ifdef DEBUG
  Serial.print("Gauge: "); Serial.print(gauge, BIN);
  Serial.print(", Angle: "); Serial.print(angle, BIN);
  Serial.print(", Quadrant: "); Serial.print(quadrant, BIN);
  Serial.print(", Assembled packet: "); Serial.println(data, BIN);
#endif
  return;
}
