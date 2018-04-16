#include "SparkFunMLX90614.h"

/* byte layout:
  * 0:0 - zero byte
  * 1:2 - Engine speed (RPM, 16 bit unsigned integer)
  * 3:3 - Wheel speed (MPH, 8 bit unsigned integer)
  * 4:4 - CVT temperature (deg F, 8 bit unsigned integer)
  * 5:5 - Oil temperature (deg F, 8 bit unsigned integer)
  * 6:7 - zero bytes
*/

// define this if you wanna read something legible on the computer
//#define SERIAL_COMPUTER

// IR thermometer setup
const uint8_t THERM_SLAVE_ADDR = 0x5A;
const uint8_t THERM_READ_TA_CMD = 0x06;
const uint8_t THERM_REPLY_BYTES = 3;
IRTherm temp;

// wheel specs, in inches
const float WHEEL_DIAMETER = 23.0;
const float WHEEL_CIRCUMFERENCE = 3.14 * WHEEL_DIAMETER;

// must be 2 or 3
const uint8_t ENGINE_PIN = 2;
const uint8_t WHEEL_PIN = 3;

// for the running average calc - POWER must be a power of 2 and ALPHA should be close to it
const unsigned long POWER = 256;
const unsigned long ALPHA = 243;

volatile uint8_t timer2_count = 0;

volatile unsigned long engine_last_pulse = 0;
volatile unsigned long engine_pulse_duration = 0;
unsigned short engine_rpm = 0;
// ms/sec * sec/min
const float ENGINE_CONVERSION_FACTOR = 1000.0 * 60.0;

volatile unsigned long wheel_last_pulse = 0;
volatile unsigned long wheel_pulse_duration = 0;
unsigned char wheel_speed = 0;
// ms/sec * sec/hr * in/rev * mile/in
const float WHEEL_CONVERSION_FACTOR = 1000.0 * 3600.0 * WHEEL_CIRCUMFERENCE * (1.0/63360.0);

// we send 8 bytes every time
uint8_t send_data[8];
void send( const uint32_t id, const uint8_t* data );

void setup() {

  // set up CAN bus interface board
#ifdef SERIAL_COMPUTER
  Serial.begin(115200);
#else
  Serial.begin(9600);
#endif

  // set up 10ms interrupt on timer2
  // set the mode - CTC
  TCCR2A |= _BV(WGM21);
  // set up the clock source (sys / 1024)
  TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
  // enable timer/counter 2 output compare A interrupt
  TIMSK2 |= _BV(OCIE2A);
  // set max to 155, yielding very slightly more than a 10ms interrupt period
  OCR2A = 0x9B;

  // set up interrupts for Hall effect sensors
  attachInterrupt(digitalPinToInterrupt(ENGINE_PIN), engine_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(WHEEL_PIN), wheel_interrupt, FALLING);

  // set up the IR thermometer
  temp.begin(THERM_SLAVE_ADDR);
  temp.setUnit(TEMP_F);
}

void loop() {
  // this will happen at most once every 100 milliseconds
  // if it takes longer than 100ms, it will run back to back
  if( timer2_count >= 10 ) {
    timer2_count = 0;

    // calculate stuff!
    engine_rpm = ENGINE_CONVERSION_FACTOR  / engine_pulse_duration;
    wheel_speed = WHEEL_CONVERSION_FACTOR * wheel_pulse_duration;
    //temp.read();

    // prepare data to send
    // buffer bytes
    send_data[0] = 0x00;
    // enigne RPM
    send_data[1] = (uint8_t)(engine_rpm >> 8);
    send_data[2] = (uint8_t)(engine_rpm >> 0);
    // wheel speed
    send_data[3] = (uint8_t)wheel_speed;
    // CVT temp
    send_data[4] = (uint8_t)temp.object();
    // oil temp
    send_data[5] = 0;
    // buffer byte
    send_data[6] = 0x00;
    send_data[7] = 0x00;

#ifdef SERIAL_COMPUTER
    //Serial.print("RPM:   "); Serial.print(engine_rpm);
    Serial.print("RPM:   "); Serial.print(engine_pulse_duration);
    Serial.print("\nSpeed: "); Serial.print(wheel_speed);
    Serial.print("\nTemp:  "); Serial.print(temp.object());
    Serial.println();
#else
    send(0x00000000, send_data);
#endif
  }
}

//  Timer 2 interrupt - this occurs every ~10ms
SIGNAL(TIMER2_COMPA_vect) {
  // doesn't seem to want to clear on compare match
  TCNT2 = 0x00;
  timer2_count++;
}

// interrupts for the Hall effect sensor
void engine_interrupt() {
  // run a very light average
  engine_pulse_duration = (ALPHA * (millis() - engine_last_pulse) + (POWER - ALPHA) * engine_pulse_duration) / POWER;
  engine_last_pulse = millis();
}

void wheel_interrupt() {
  // run a very light average
  wheel_pulse_duration = (ALPHA * (millis() - wheel_last_pulse) + (POWER - ALPHA) * wheel_pulse_duration) / POWER;
  wheel_last_pulse = millis();
}

// send data over the can bus
void send( const uint32_t id, const uint8_t* data ) {
  // see http://docs.longan-labs.cc/can_bus/#send

  // ID bytes
  Serial.write((uint8_t)(id >> 24));
  Serial.write((uint8_t)(id >> 16));
  Serial.write((uint8_t)(id >> 8));
  Serial.write((uint8_t)(id >> 0));

  // extended, remote
  Serial.write((uint8_t)0x00);
  Serial.write((uint8_t)0x00);

  // data (MUST write 8 bytes)
  Serial.write(data, 8);
  
  return;
}
