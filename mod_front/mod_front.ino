#include <Servo.h>

/* byte layout:
  * 0:0 - zero byte
  * 1:2 - Engine speed (RPM, 16 bit unsigned integer)
  * 3:3 - Wheel speed (MPH, 8 bit unsigned integer)
  * 4:4 - CVT temperature (deg F, 8 bit unsigned integer)
  * 5:5 - Oil temperature (deg F, 8 bit unsigned integer)
  * 6:7 - zero bytes
*/

// timing stuff
unsigned long  current_millis = 0;
unsigned long  last_can_trigger = 0;
unsigned long  last_servo_trigger = 0;

// servo stuff
// by only instantiating one object, we guarantee we only drive one servo at a time
const uint8_t SERVO_PINS[] = {3, 5};
const uint8_t CVT_TEMP_SERVO_INDEX = 0;
const uint8_t WHEEL_SPEED_SERVO_INDEX = 1;
const uint8_t NUMBER_OF_SERVOS = 2;
// amount of time between servo position updates
const uint16_t SERVO_REFRESH_DELAY = 100;
uint8_t current_servo = 0;
Servo servo;

// CAN stuff
// amount of time between checking for new CAN messages
uint8_t CAN_POLL_DELAY = 250;
const uint32_t MODULE_ID = 0x00000000;
void receive( uint16_t* engine_speed, uint8_t* wheel_speed, uint8_t* cvt_temp, uint8_t* oil_temp);

// storage variables
uint16_t engine_speed = 0;
uint8_t wheel_speed = 0;
uint8_t cvt_temp = 0;
uint8_t oil_temp = 0;

// calculation and range management
const uint8_t MIN_CVT_TEMP = 0;
const uint8_t MAX_CVT_TEMP = 250;
const uint8_t MIN_CVT_SERVO_ANGLE = 15;
const uint8_t MAX_CVT_SERVO_ANGLE = 165;
const uint8_t MIN_WHEEL_SPEED = 0;
const uint8_t MAX_WHEEL_SPEED = 65;
const uint8_t MIN_WHEEL_SERVO_ANGLE = 15;
const uint8_t MAX_WHEEL_SERVO_ANGLE = 165;
uint8_t servo_angle = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // the timing really doesn't have to be too tight here
  current_millis = millis();

  // messages should come in about every 100 ms; we'll check about 3 times faster just to make 
  // sure we don't miss anything!
  if( (current_millis - last_can_trigger) >  CAN_POLL_DELAY) {
    last_can_trigger = current_millis;
    //receive(&engine_speed, &wheel_speed, &cvt_temp, &oil_temp);
  }

  // each time this runs it updates a different servo
  if( (current_millis - last_servo_trigger) > (SERVO_REFRESH_DELAY / NUMBER_OF_SERVOS) ) {
    last_servo_trigger = current_millis;
    servo.detach();
    if( ++current_servo == NUMBER_OF_SERVOS ) {
      current_servo = 0;
    }
    servo.attach(SERVO_PINS[current_servo]);

    switch( current_servo ) {
      case CVT_TEMP_SERVO_INDEX:
        cvt_temp = constrain(cvt_temp, MIN_CVT_TEMP, MAX_CVT_TEMP);
        servo_angle = map(cvt_temp, MIN_CVT_TEMP, MAX_CVT_TEMP, MIN_CVT_SERVO_ANGLE, MAX_CVT_SERVO_ANGLE);
        break;

      case WHEEL_SPEED_SERVO_INDEX:
        wheel_speed = constrain(wheel_speed, MIN_WHEEL_SPEED, MAX_WHEEL_SPEED);
        servo_angle = map(wheel_speed, MIN_WHEEL_SPEED, MAX_WHEEL_SPEED, MIN_WHEEL_SERVO_ANGLE, MAX_WHEEL_SERVO_ANGLE);
        break;
    }

    Serial.print("servo: "); Serial.println(current_servo);
    Serial.print("angle: "); Serial.println(servo_angle);

    servo.write(servo_angle);
  }
}

void consume_serial() {
  // flush the serial buffer
  while( Serial.available() > 0 ) {
    Serial.read();
  }
}

// if the recieved packet is not valid or not adressed to us, the values of the args don't change
void receive( uint16_t* engine_speed, uint8_t* wheel_speed, uint8_t* cvt_temp, uint8_t* oil_temp) {
  // see http://docs.longan-labs.cc/can_bus/#recv

  // we need a full message
  if( Serial.available() < 12 ) {
    consume_serial();
    return;
  }

  // ID bytes
  uint32_t id;
  id |= ((uint32_t)Serial.read()) << 24;
  id |= ((uint32_t)Serial.read()) << 16;
  id |= ((uint32_t)Serial.read()) << 8;
  id |= ((uint32_t)Serial.read()) << 0;

  // do we need to care about this message?
  if( id != MODULE_ID ) {
    consume_serial();
    return;
  }

  // data (MUST read 8 bytes)
  // zero byte
  if( Serial.read() != 0 ) {
    consume_serial();
    return;
  }
  // engine RPM (16 bit)
  *engine_speed = ((uint16_t)Serial.read()) << 8;
  *engine_speed |= Serial.read();
  // wheel MPH
  *wheel_speed = Serial.read();
  // CVT *F
  *cvt_temp = Serial.read();
  // Oil *F
  *oil_temp = Serial.read();
  // zero bytes
  Serial.read();
  Serial.read();
}
