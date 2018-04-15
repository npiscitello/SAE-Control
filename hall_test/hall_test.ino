#define IN_PIN 7
#define LED_PIN 13

void setup() {
  pinMode(IN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, digitalRead(IN_PIN));
}
