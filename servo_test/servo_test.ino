#include <Servo.h>

#define SERVO_PIN 6
Servo servo;

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
}

void loop() {

  /*
  for( int pos = 5; pos < 170; pos++ ) {
    servo.write(pos);
    delay(10);
  }

  for( int pos = 170; pos > 5; pos-- ) {
    servo.write(pos);
    delay(10);
  }
  */

  // I can get away with a bunch of servos on one arduino power supply
  // IF I only have one of them turned on at a time!
  // maybe only turn a gauge when there's more than 5 degrees of change or something?
  // run a half second update loop?

  servo.attach(SERVO_PIN);
  servo.write(20);
  delay(5000);
  servo.write(160);
  delay(5000);
  servo.detach();
  digitalWrite(SERVO_PIN, LOW);
  delay(10000);
}
