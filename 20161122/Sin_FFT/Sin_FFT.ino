/*
 * File: backdoor.ino
 * 
 * Author: Jiankai Li
 * Date: 2016-11-22
 */
#include <avr/wdt.h>
#include <TimerOne.h>


#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 128 point fht

#include <FHT.h> // include the library
#include <math.h>

void setup() {

    Serial.begin(115200);
    Serial.println("power on!");

    wdt_enable(WDTO_2S);
     
    Timer1.initialize(1000000); // set a timer of length 1000000 microseconds 1 second
    Timer1.attachInterrupt( timerIsrFeedFog ); // attach the service routine here
    wdt_reset();
}

void loop() {
  while(1) { // reduces jitter
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      fht_input[i] = 100 + (sin(2*M_PI*i)*100);
      // fht_input[i] = analogRead(0);; // put real data into bins
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

