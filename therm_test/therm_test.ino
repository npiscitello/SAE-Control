const uint8_t THERM_SLAVE_ADDR = 0x5A;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(950);
}
