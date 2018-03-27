// this is the transmitter/receiver switch: one Arduino 
// should be a transmitter, the other should receive
#define RCV

#ifdef RCV
uint16_t rcv = 0;
#else
uint16_t send = 0;
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
  if( Serial.available() == 2 ) {
    rcv = Serial.read() << 8;
    rcv |= Serial.read();

    if( rcv > 512 ) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    delay(10);
  }
#else
  send = analogRead(A0);
  // this should send 2 bytes, high first
  Serial.write((uint8_t)(send >> 8));
  Serial.write((uint8_t)send);
  delay(50);
#endif
}
