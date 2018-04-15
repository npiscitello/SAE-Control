#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(9);
}

void loop() {

  for( int pos = 5; pos < 170; pos++ ) {
    servo.write(pos);
    delay(10);
  }

  for( int pos = 170; pos > 5; pos-- ) {
    servo.write(pos);
    delay(10);
  }
}
