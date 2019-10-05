#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <SimpleServo.h>

SimpleServo servo;

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define CENTER_ALIGMENT 80
#define MOVEMENT_ANGLE 30

void my_sleep(int sec){
  for (int i=0; i <= sec; i++){
    sleep_enable();
    sleep_cpu();
  }
}

void setup() {
  // Power Saving setup
  for (byte i = 1; i < 6; i++) {
    pinMode(i, INPUT);      // Set all ports as INPUT to save energy
    digitalWrite (i, LOW);  //
  }
  adc_disable();          // Disable Analog-to-Digital Converter
 
  wdt_reset();            // Watchdog reset
  wdt_enable(WDTO_1S);    // Watchdog enable Options: 15MS, 30MS, 60MS, 120MS, 250MS, 500MS, 1S, 2S, 4S, 8S
  WDTCR |= _BV(WDIE);     // Interrupts watchdog enable
  sei();                  // enable interrupts
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Sleep Mode: max
}

void loop() {
  servo.attach(0);

  servo.write(CENTER_ALIGMENT);
  servo.write(CENTER_ALIGMENT - MOVEMENT_ANGLE);
  servo.write(CENTER_ALIGMENT);
 
  my_sleep(60);
   
  servo.attach(0);
  servo.write(CENTER_ALIGMENT);
  servo.write(CENTER_ALIGMENT + MOVEMENT_ANGLE);
  servo.write(CENTER_ALIGMENT);
 
  my_sleep(900);
}
 
ISR (WDT_vect) {
  WDTCR |= _BV(WDIE);
}
