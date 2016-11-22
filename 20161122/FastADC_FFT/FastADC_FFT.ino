/*
 * File: backdoor.ino
 * 
 * Author: Jiankai Li
 * Date: 2016-11-22
 */
#include <avr/wdt.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>



#define PIXIEL_PIN 6
#define PIXIEL_NUM 30

#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define LOG_OUT 1 // use the log output function
#define FHT_N 128 // set to 256 point fht

#include <FHT.h> // include the library

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXIEL_NUM, PIXIEL_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
    int start ;
    int i ;
    
    Serial.begin(115200);
    Serial.println("power on!");

    wdt_enable(WDTO_2S);
     
    Timer1.initialize(1000000); // set a timer of length 1000000 microseconds 1 second
    Timer1.attachInterrupt( timerIsrFeedFog ); // attach the service routine here
    wdt_reset();
      
      // strip.begin();
      // strip.show(); // Initialize all pixels to 'off'
      // for (int i = 0; i<PIXIEL_NUM; i++) {
            // strip.setPixelColor(i,0xA0,0xA0,00);
            // strip.show();
      // }

     
    #if FASTADC
     // set prescale to 16
    sbi(ADCSRA,ADPS2) ;
    cbi(ADCSRA,ADPS1) ;
    cbi(ADCSRA,ADPS0) ;
    #endif
    Serial.print("ADCTEST: ") ;
    start = millis() ;
    for (i = 0 ; i < 1000 ; i++)
       analogRead(0) ;
    Serial.print(millis() - start) ;
    Serial.println(" msec (1000 calls)") ;
}

void loop() {
  while(1) { // reduces jitter

    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      
      fht_input[i] = analogRead(0);; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    Serial.println("start");
    for (uint8_t i = 0 ; i < FHT_N/2 ; i++) {
      Serial.print(fht_log_out[i]); // send out the data
      Serial.print(" "); // send out the data
    }
    Serial.println(" ");
  }
}
void timerIsrFeedFog()
{
    wdt_reset();
    Serial.println("------------Time ISR");
}

