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

void delay_tone(uint16_t duration){
	delay_us(duration * 1000000);
}

// define some notes
// Frequencies from http://www.phy.mtu.edu/~suits/notefreqs.html
// converted to half-periods (us) by calculating
// 	1000000/2/frequency
// where frequency is in Hz


//#define E6 379
//#define G6 319
//#define AS5 536
//#define A6 284
//#define F6 358
/*
#define E4 1517
#define A4 1136
#define B4 1012
#define AS4 1073

#define F4 1432
#define G4 1276
#define A5 568
#define B5 506
 #define C5 956
#define D5 851
#define E5 758
 #define F5 716
#define FS5 675
#define G5 637
#define C6 477
#define D6 425
*/
/*************************************************
 * Public Constants
 *************************************************/

#define NOTE_B0  500000/31
#define NOTE_C1  500000/33
#define NOTE_CS1 500000/35
#define NOTE_D1  500000/37
#define NOTE_DS1 500000/39
#define NOTE_E1  500000/41
#define NOTE_F1  500000/44
#define NOTE_FS1 500000/46
#define NOTE_G1  500000/49
#define NOTE_GS1 500000/52
#define NOTE_A1  500000/55
#define NOTE_AS1 500000/58
#define NOTE_B1  500000/62
#define NOTE_C2  500000/65
#define NOTE_CS2 500000/69
#define NOTE_D2  500000/73
#define NOTE_DS2 500000/78
#define NOTE_E2  500000/82
#define NOTE_F2  500000/87
#define NOTE_FS2 500000/93
#define NOTE_G2  500000/98
#define NOTE_GS2 500000/104
#define NOTE_A2  500000/110
#define NOTE_AS2 500000/117
#define NOTE_B2  500000/123
#define NOTE_C3  500000/131
#define NOTE_CS3 500000/139
#define NOTE_D3  500000/147
#define NOTE_DS3 500000/156
#define NOTE_E3  500000/165
#define NOTE_F3  500000/175
#define NOTE_FS3 500000/185
#define NOTE_G3  500000/196
#define NOTE_GS3 500000/208
#define NOTE_A3  500000/220
#define NOTE_AS3 500000/233
#define NOTE_B3  500000/247
#define NOTE_C4  500000/262
#define NOTE_CS4 500000/277
#define NOTE_D4  500000/294
#define NOTE_DS4 500000/311
#define NOTE_E4  500000/330
#define NOTE_F4  500000/349
#define NOTE_FS4 500000/370
#define NOTE_G4  500000/392
#define NOTE_GS4 500000/415
#define NOTE_A4  500000/440
#define NOTE_AS4 500000/466
#define NOTE_B4  500000/494
#define NOTE_C5  500000/523
#define NOTE_CS5 500000/554
#define NOTE_D5  500000/587
#define NOTE_DS5 500000/622
#define NOTE_E5  500000/659
#define NOTE_F5  500000/698
#define NOTE_FS5 500000/740
#define NOTE_G5  500000/784
#define NOTE_GS5 500000/831
#define NOTE_A5  500000/880
#define NOTE_AS5 500000/932
#define NOTE_B5  500000/988
#define NOTE_C6  500000/1047
#define NOTE_CS6 500000/1109
#define NOTE_D6  500000/1175
#define NOTE_DS6 500000/1245
#define NOTE_E6  500000/1319
#define NOTE_F6  500000/1397
#define NOTE_FS6 500000/1480
#define NOTE_G6  500000/1568
#define NOTE_GS6 500000/1661
#define NOTE_A6  500000/1760
#define NOTE_AS6 500000/1865
#define NOTE_B6  500000/1976
#define NOTE_C7  500000/2093
#define NOTE_CS7 500000/2217
#define NOTE_D7  500000/2349
#define NOTE_DS7 500000/2489
#define NOTE_E7  500000/2637
#define NOTE_F7  500000/2794
#define NOTE_FS7 500000/2960
#define NOTE_G7  500000/3136
#define NOTE_GS7 500000/3322
#define NOTE_A7  500000/3520
#define NOTE_AS7 500000/3729
#define NOTE_B7  500000/3951
#define NOTE_C8  500000/4186
#define NOTE_CS8 500000/4435
#define NOTE_D8  500000/4699
#define NOTE_DS8 500000/4978



#define DUR 40

#define NOTE 300
#define NOTE2 NOTE/2.0
#define NOTE4 NOTE/4.0
#define NOTE8 NOTE/8.0
#define NOTE16 NOTE/16.0
#define NOTE32 NOTE/32.0

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
	16d6,8p,
	16c6
	*/
	
	//16e6,16e6,32p,
	play_tone(NOTE_E5, NOTE16);
	play_tone(NOTE_E5, NOTE16);
	delay_tone(NOTE32);
	
	//8e6,16c6,8e6,8g6,8p,
	play_tone(NOTE_E5, NOTE8);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_E5, NOTE8);
	play_tone(NOTE_G5, NOTE8);
	delay_tone(NOTE8);
	
	//8g,8p,
	play_tone(NOTE_G5, NOTE8);
	delay_tone(NOTE8);
	
	//8c6,16p,
	play_tone(NOTE_C5, NOTE8);
	delay_tone(NOTE16);
	
	//8g,16p,
	play_tone(NOTE_G4, NOTE8);
	delay_tone(NOTE16);
	
	//8e,16p,
	play_tone(NOTE_E4, NOTE8);
	delay_tone(NOTE16);
	
	//8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,
	play_tone(NOTE_A4, NOTE8);
	play_tone(NOTE_B4, NOTE8);
	play_tone(NOTE_AS4, NOTE16);
	play_tone(NOTE_A4, NOTE8);
	play_tone(NOTE_G4, NOTE16+NOTE32);
	play_tone(NOTE_E5, NOTE16);
	play_tone(NOTE_G5, NOTE16);
	play_tone(NOTE_A5, NOTE8);
	play_tone(NOTE_F5, NOTE16);
	play_tone(NOTE_G5, NOTE8);
	play_tone(NOTE_E5, NOTE8);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_D5, NOTE16);
	play_tone(NOTE_B4, NOTE8);
	delay_tone(NOTE16);
	
	//8c6,16p,
	play_tone(NOTE_C5, NOTE8);
	delay_tone(NOTE16);
	
	//8g,16p,
	play_tone(NOTE_G4, NOTE8);
	delay_tone(NOTE16);
	
	//8e,16p,
	play_tone(NOTE_E4, NOTE8);
	delay_tone(NOTE16);
	
	//8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,
	play_tone(NOTE_A4, NOTE8);
	play_tone(NOTE_B4, NOTE8);
	play_tone(NOTE_AS4, NOTE16);
	play_tone(NOTE_A4, NOTE8);
	play_tone(NOTE_G4, NOTE16+NOTE32);
	play_tone(NOTE_E5, NOTE16);
	play_tone(NOTE_G5, NOTE16);
	play_tone(NOTE_A5, NOTE8);
	play_tone(NOTE_F5, NOTE16);
	play_tone(NOTE_G5, NOTE8);
	play_tone(NOTE_E5, NOTE8);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_D5, NOTE16);
	play_tone(NOTE_B4, NOTE8);
	delay_tone(NOTE16);
	
	//16g6,16f#6,16f6,16d#6,16p,
	play_tone(NOTE_G5, NOTE16);
	play_tone(NOTE_FS5, NOTE16);
	play_tone(NOTE_F5, NOTE16);
	play_tone(NOTE_DS5, NOTE16);
	delay_tone(NOTE16);
	
	//16e6,16p,
	play_tone(NOTE_E5, NOTE16);
	delay_tone(NOTE16);
	
	//16g#,16a,16c6,16p,
	play_tone(NOTE_GS4, NOTE16);
	play_tone(NOTE_A4, NOTE16);
	play_tone(NOTE_C5, NOTE16);
	delay_tone(NOTE16);
	
	//16a,16c6,16d6,8p,
	play_tone(NOTE_A4, NOTE16);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_D5, NOTE16);
	delay_tone(NOTE8);
	
	//16g6,16f#6,16f6,16d#6,16p,
	play_tone(NOTE_G5, NOTE16);
	play_tone(NOTE_FS5, NOTE16);
	play_tone(NOTE_F5, NOTE16);
	play_tone(NOTE_DS5, NOTE16);
	delay_tone(NOTE16);
	
	//16e6,16p,
	play_tone(NOTE_E5, NOTE16);
	delay_tone(NOTE16);
	
	//16c7,16p,
	play_tone(NOTE_C6, NOTE16);
	delay_tone(NOTE16);
	
	//16c7,16c7,p,
	play_tone(NOTE_C6, NOTE16);
	play_tone(NOTE_C6, NOTE16);
	delay_tone(NOTE);
	
	//16g6,16f#6,16f6,16d#6,16p,
	play_tone(NOTE_G5, NOTE16);
	play_tone(NOTE_FS5, NOTE16);
	play_tone(NOTE_F5, NOTE16);
	play_tone(NOTE_DS5, NOTE16);
	delay_tone(NOTE16);
	
	//16e6,16p,
	play_tone(NOTE_E5, NOTE16);
	delay_tone(NOTE16);
	
	//16g#,16a,16c6,16p,
	
	//16a,16c6,16d6,8p,
	play_tone(NOTE_A4, NOTE16);
	play_tone(NOTE_C5, NOTE16);
	play_tone(NOTE_D5, NOTE16);
	delay_tone(NOTE8);
	
	//16d#6,8p,
	play_tone(NOTE_DS5, NOTE16);
	delay_tone(NOTE8);
	
	//16d6,8p,
	play_tone(NOTE_D5, NOTE16);
	delay_tone(NOTE8);
	
	//16c6
	play_tone(NOTE_C5, NOTE16);
	
  
    // delay a bit
    delay_ms(500);
  }
  
  return 0;
}
