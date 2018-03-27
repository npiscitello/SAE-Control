// this is the transmitter/receiver switch: one Arduino 
// should be a transmitter, the other should receive
//#define RCV

#ifdef RCV
uint16_t receive();
#else
void send( uint32_t id, uint8_t ext, uint8_t rtr, uint16_t data );
#endif

void setup() {
  Serial.begin(9600);
#ifdef RCV
  pinMode(LED_BUILTIN, OUTPUT);
#else
#endif
}

void loop() {
#ifdef RCV
  // there's a message waiting!
  if( Serial.available() == 12 ) {
    if( receive() > 512 ) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    delay(10);
  }
#else
  send( 0x00000000, (uint16_t)analogRead(A0));
  delay(50);
#endif
}

#ifdef RCV
uint16_t receive() {
  // see http://docs.longan-labs.cc/can_bus/#recv

  // ID bytes
  Serial.read();
  Serial.read();
  Serial.read();
  Serial.read();

  // data
  uint16_t retval = 0x0000;
  retval |= (Serial.read() << 8);
  retval |= (Serial.read() << 0);
  Serial.read();
  Serial.read();
  Serial.read();
  Serial.read();
  Serial.read();
  Serial.read();

  return retval;
}
#else
void send( uint32_t id, uint16_t data ) {
  // see http://docs.longan-labs.cc/can_bus/#send

  // ID bytes
  Serial.write((uint8_t)(id >> 24));
  Serial.write((uint8_t)(id >> 16));
  Serial.write((uint8_t)(id >> 8));
  Serial.write((uint8_t)(id >> 0));

  // extended, remote
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);

  // data
  Serial.write((uint8_t)(data >> 8));
  Serial.write((uint8_t)(data >> 0));
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);
  
  return;
}
#endif
