//////////////////////////////////////////////////////////////
// Digispark BakerMIDI
//////////////////////////////////////////////////////////////
// ATTiny85 
//                      +------+
//             D5 | A0 -|      |- VCC
//    (USB D-) D3 | A3 -|      |- A1 / D2 (Pot)
//    (USB D+) D4 | A2 -|      |- D1      (LED)    
//                 GND -|      |- D0      
//                      +------+
//
//////////////////////////////////////////////////////////////

#include "DigisparkMIDI.h"
#include <avr/wdt.h>

#define PORTD0 0x01
#define PORTD1 0x02
#define PORTD2 0x04
#define PORTD3 0x08
#define PORTD4 0x10
#define PORTD5 0x20

int analogReading =0;         // P5 Thinpot
int channel = 0;
int note = 0;
int notePrevious = note;

void setup() {
  wdt_enable(WDTO_2S);

  DDRB |= (PORTD1);              //Led port
  pinMode(2, INPUT);             //Input on P2 (A1)
  
  // reset LED 
  PORTB |= PORTD1;
  delayMs(200);
  PORTB &= ~PORTD1;
  delayMs(200);
  PORTB |= PORTD1;
  delayMs(200);
  PORTB &= ~PORTD1;
  delayMs(200);
  PORTB |= PORTD1;
  delayMs(200);
  PORTB &= ~PORTD1;
  
  TIMSK &= ~(1 << TOIE0);          //  disable timer 1 Interrupt

  UsbMidi.init();
}

void loop() {
  int i =0;
  wdt_reset();  
  UsbMidi.update();
  analogReading =  (analogRead(1) >> 2);       //Read P2
  note = 48;
  
  //Note detection
  while(analogReading > i){       //looks for the note 
    note++;
    i=i+7;
  }
  
  //note on command
  if(notePrevious != note){   //update MIDI note
    UsbMidi.sendNoteOn(channel,note,100);
  }
  
  //note off command
  if(notePrevious != note){
    UsbMidi.sendNoteOff(channel,notePrevious);
  }
  notePrevious = note;
}
 
void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
    UsbMidi.update();
  }
}
