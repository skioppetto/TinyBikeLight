#include <avr/sleep.h>

#ifdef __AVR_ATtiny13__

  #define SLEEP_FOREVER  128
  #define SLEEP_016MS    (period_t)0
  #define SLEEP_125MS    (1<<WDP1) | (1<<WDP0)
  #define SLEEP_250MS    (1<<WDP2)
  #define SLEEP_500MS    (1<<WDP2) | (1<<WDP0)
  #define SLEEP_1SEC     (1<<WDP2) | (1<<WDP1)
  #define SLEEP_2SEC     (1<<WDP2) | (1<<WDP1) | (1<<WDP0)
  #define SLEEP_4SEC     (1<<WDP3)
  #define SLEEP_8SEC     (1<<WDP3) | (1<<WDP0)

#endif /* ifdef __AVR_ATtiny13__ */


const byte LED = 0;  // pin 2
const byte SWITCH = 4; // pin 3 / PCINT4


  void sleepNow(byte b) {
    {
      ACSR |= (1 << ACD); //Analog comparator off
      ACSR = ADMUX = ADCSRA = 0;
    }

    if (b != SLEEP_FOREVER) {
      WDTCR |= b;    //Watchdog
      // Enable watchdog timer interrupts
      WDTCR |= (1 << WDTIE);
    }
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    cli(); // No interrupts; timed sequence
    sei(); // Enable global interrupts or we never wake
    sleep_mode();
    sleep_disable();
  }


  // calling in loop() for example: sleepNow(SLEEP_1SEC);
  void setup ()
  {
  pinMode (LED, OUTPUT);
  pinMode (SWITCH, INPUT);
  digitalWrite (SWITCH, HIGH);  // internal pull-up
  
  // pin change interrupt (example for D4)
  PCMSK  |= bit (PCINT4);  // want pin D4 / pin 3
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts
  GIMSK  |= bit (PCIE);    // enable pin change interrupts 
  
  }  // end of setup

void loop ()
  {
  digitalWrite (LED, HIGH);
  delay (500); 
  digitalWrite (LED, LOW);
  delay (500); 
  sleepNow (SLEEP_FOREVER);
  }  // end of loop
