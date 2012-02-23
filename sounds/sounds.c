// musicbox1.c
// for NerdKits with ATtiny26L
// mrobbins@mit.edu

// F_CPU defined for delay.c
#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"

// PIN DEFINITIONS:
//
// PA0 -- temperature sensor analog input
// PA1 -- piezo buzzer
// PA4 -- LCD RS (pin 4)
// PA5 -- LCD E (pin 6)
// PA7 -- button (pullup high)
// PB3-6 -- LCD DB4-7 (pins 11-14)

void lcd_blank_line() {
  lcd_write_string(PSTR("                        "));
}

void lcd_quick_clear_and_home() {
    lcd_home();
    lcd_blank_line();
    lcd_line_two();
    lcd_blank_line();
    lcd_home();
}

void play_tone(uint16_t delay, uint8_t duration) {
  // delay is half-period in microseconds
  // duration is in 10ms increments
  
  // example: 440Hz --> delay=1136
  
  
  // duration = 2*delay * cycles (all in same units)
  // cycles = 10000 * duration / delay / 2
  // cycles = 100 * duration / (delay/50)
  uint16_t tmp = 100 * duration;
  uint16_t delaysm = delay / 50;
  uint16_t cycles = tmp / delaysm;
  
  while(cycles > 0) {
    PORTC |= (1<<PC1);
    delay_us(delay);
    PORTC &= ~(1<<PC1);
    delay_us(delay);
    cycles--;
  }
  delay_us(400);
}

// define some notes
// Frequencies from http://www.phy.mtu.edu/~suits/notefreqs.html
// converted to half-periods (us) by calculating
// 	1000000/2/frequency
// where frequency is in Hz
#define F4 1432
#define G4 1276
#define A5 568
#define B5 506
 #define C5 956
#define D5 851
#define E5 758
 #define F5 716
#define Fsh5 675
#define G5 637
#define C6 477
#define D6 425

#define DUR 40

#define NOTE 400
#define NOTE2 NOTE/2
#define NOTE4 NOTE/4
#define NOTE8 NOTE/8
#define NOTE16 NOTE/16
#define NOTE32 NOTE/32

int main() {
  // internal RC oscillator calibration for 8MHz.
  //OSCCAL = 176;

  // enable the piezo as output
  DDRC |= (1<<PC1);
  
  // enable internal pullup on PA7 (the button)
  PORTC |= (1<<PC4);

  // fire up the LCD
  lcd_init();
  lcd_home();

  // loop forever!
  while(1) {
    lcd_quick_clear_and_home();
    lcd_write_string(PSTR("Press to play a song"));
  
    // wait for button press...
    while(PINC & (1<<PC4)) {
      // do nothing
    }
	/*
	16e6,16e6,32p,
	8e6,16c6,8e6,8g6,8p,
	8g,8p,
	8c6,16p,
	8g,16p,
	8e,16p,
	8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,
	8c6,16p,
	8g,16p,
	8e,16p,
	8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,
	16g6,16f#6,16f6,16d#6,16p,
	16e6,16p,
	16g#,16a,16c6,16p,
	16a,16c6,16d6,8p,
	16g6,16f#6,16f6,16d#6,16p,
	16e6,16p,
	16c7,16p,
	16c7,16c7,p,
	16g6,16f#6,16f6,16d#6,16p,
	16e6,16p,
	16g#,16a,16c6,16p,
	16a,16c6,16d6,8p,
	16d#6,8p,
	16d6,8p
	,16c6
	*/
	
	//16e6,16e6,32p,
	play_tone(E5, NOTE16);
	play_tone(E5, NOTE16);
	delay_us(NOTE32);
	
	//8e6,16c6,8e6,8g6,8p,
	play_tone(E5, NOTE8);
	play_tone(C5, NOTE16);
	play_tone(E5, NOTE8);
	play_tone(G5, NOTE8);
	delay_us(NOTE8);
	
	//8g,8p,
	play_tone(G4, NOTE8);
	delay_us(NOTE8);
	
	//8c6,16p,
	play_tone(C5, NOTE8);
	delay_us(NOTE16);
	
	
	/*play_tone(E5, NOTE8);
	play_tone(E5, NOTE8);
	delay_us(NOTE8);
	play_tone(E5, NOTE8);
	delay_us(NOTE8);
	play_tone(C5, NOTE8);
	play_tone(E5, NOTE4);
	play_tone(G5, NOTE4);
	delay_us(NOTE4);
	play_tone(G4, NOTE4);
	delay_us(NOTE4);
	play_tone(C5, NOTE4+NOTE8);
	play_tone(G4, NOTE8);
	delay_us(NOTE4);
	play_tone(F4, NOTE8+NOTE4);
	play_tone(G4, NOTE4);
	play_tone(B4, NOTE8);
	//play_tone(B_5, NOTE8); //FIX
	//play_tone(A5, NOTE8);
	*/
	
	
    /*play_tone(D5, DUR);
    play_tone(E5, DUR);
    play_tone(D5, DUR);
    play_tone(G5, DUR);
    play_tone(Fsh5, 2*DUR);
	
    play_tone(D5, DUR);
    play_tone(E5, DUR);
    play_tone(D5, DUR);
    play_tone(A5, DUR);
    play_tone(G5, 2*DUR);

    play_tone(D5, DUR);
    play_tone(D6, DUR);
    play_tone(B5, DUR);
    play_tone(G5, DUR);
    play_tone(Fsh5, DUR);
    play_tone(E5, DUR);
    
    play_tone(C6, DUR);
    play_tone(B5, DUR);
    play_tone(G5, DUR);
    play_tone(A5, DUR);
    play_tone(G5, 2*DUR);*/
  
    // delay a bit
    delay_ms(500);
  }
  
  return 0;
}
