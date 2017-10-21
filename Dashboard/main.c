#include <avr/io.h>
#include <avr/delay.h>

int main(void) {

  // set up timers for PWM - write to OCR0A, OCR0B to set PWM duty cycle
  DDRD = _BV(DDD5) | _BV(DDD6);
  PRR = ~_BV(PRTIM0) & ~_BV(PRTIM2);
  //TCCR0A = (uint8_t)((_BV(WGM00) | _BV(WGM01) | _BV(COM0A1) | _BV(COM0B1)) & ~_BV(COM0A0) & ~_BV(COM0B0));  // Both are regular
  TCCR0A = (uint8_t)((_BV(WGM00) | _BV(WGM01) | _BV(COM0A1) | _BV(COM0B0) | _BV(COM0B1)) & ~_BV(COM0A0));  // A is regular, B is inverse
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02) & ~_BV(CS01)) | _BV(CS00)); // PWM freq div1 - annoying high pitched hum
  TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02) & ~_BV(CS00)) | _BV(CS01)); // PWM freq div8 - almost inaudible low hum
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02)) | _BV(CS01) | _BV(CS00));  // PWM freq div64 - visible choppiness
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS00) & ~_BV(CS01)) | _BV(CS02)); // PWM freq div256 - unusable
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS01)) | _BV(CS02) | _BV(CS00));  // PWM freq div1024 - unusable
  DDRB = _BV(DDB3);
  TCCR2A = (uint8_t)((_BV(WGM20) | _BV(WGM21) | _BV(COM2A1)) & ~_BV(COM2A0));
  TCCR2B = (uint8_t)((~_BV(WGM22) & ~_BV(FOC2A) & ~_BV(FOC2B) & ~_BV(CS22) & ~_BV(CS20)) | _BV(CS21));
  // not quite half because 0xFF doesn't quite output 5V due to voltage drop in the transistor
  OCR2A = 0x76;

  while(1) {
    //OCR0A = 0xFF;
    //OCR0B = 0x00;
    ///*
    for( int i = 0; i <= 255; i++ ) {
      OCR0A = i; OCR0B = i;
      _delay_us(1000);
    }

    for( int i = 255; i >= 0; i-- ) {
      OCR0A = i; OCR0B = i;
      _delay_us(1000);
    }
    // */
  }
}
