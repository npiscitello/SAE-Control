#include <avr/io.h>
#include <avr/delay.h>

int main(void) {

  // set up timer0 for PWM - write to OCR0A and OCR0B to set PWM duty cycle
  DDRD = _BV(DDB5) | _BV(DDB6);
  PRR = ~_BV(PRTIM0);
  TCCR0A = (uint8_t)((_BV(WGM00) | _BV(WGM01) | _BV(COM0B1) | _BV(COM0B0) | _BV(COM0A1)) & ~_BV(COM0A0));
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02) & ~_BV(CS01)) | _BV(CS00)); // PWM freq div1 - annoying high pitched hum
  TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02) & ~_BV(CS00)) | _BV(CS01)); // PWM freq div8 - almost inaudible low hum
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS02)) | _BV(CS01) | _BV(CS00));  // PWM freq div64 - visible choppiness
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS00) & ~_BV(CS01)) | _BV(CS02)); // PWM freq div256 - unusable
  //TCCR0B = (uint8_t)((~_BV(WGM02) & ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(CS01)) | _BV(CS02) | _BV(CS00));  // PWM freq div1024 - unusable

  while(1) {
        for( int i = 0; i <= 255; i++ ) {
      OCR0A = i; OCR0B = i;
      _delay_us(100);
    }

    for( int i = 255; i >= 0; i-- ) {
      OCR0A = i; OCR0B = i;
      _delay_us(100);
    }
  }
}
