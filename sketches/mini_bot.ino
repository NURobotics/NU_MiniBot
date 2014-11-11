#include <avr/power.h>

int angle = 6;

void setup(){
  if (F_CPU == 8000000) clock_prescale_set(clock_div_1);
  DDRB= (1 << 2);  //port A7 is an output
  DDRA = (1 << 7);
  PORTA = 0xff;
  PORTB = 0xff;
  //DDRA = 0xFF;

  //OCR0B = 255;
  // set PWM for 50% duty cycle

  TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  // set non-inverting mode
  // set fast PWM Mode

  TCCR0B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
  
  // 157
  OCR0A = 110;
  OCR0B = angle;
  

  // set prescaler to 8 and starts PWM
  
  /*pinMode(3, OUTPUT); // output pin for OCR2B
  pinMode(5, OUTPUT); // output pin for OCR0B

  // Set up the 250KHz output
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 63;
  OCR2B = 0;

  // Set up the 8MHz output
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(WGM02) | _BV(CS00);
  OCR0A = 1;
  OCR0B = 0;*/
 }

void loop() {
  angle = ((angle-5) % 13)+6;
  delay(15);
  OCR0B = angle;
}
