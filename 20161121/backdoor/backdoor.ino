/*
 * File: backdoor.ino
 * 
 * Author: Jiankai Li
 * Date: 2016-11-18
 */

#include <avr/wdt.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>



#define PIXIEL_PIN 6
#define PIXIEL_NUM 30
#define RECORDER_PIN 4


#define Magnetic_PIN 2

#define TEST_PIN  4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXIEL_NUM, PIXIEL_PIN, NEO_GRB + NEO_KHZ800);

uint8_t flag;

void setup() {
  Serial.begin(115200);
  Serial.println("power on!");

  wdt_enable(WDTO_2S);
 
  Timer1.initialize(1000000); // set a timer of length 1000000 microseconds 1 second
  Timer1.attachInterrupt( timerIsrFeedFog ); // attach the service routine here
  wdt_reset();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for (int i = 0; i<PIXIEL_NUM; i++) {
        strip.setPixelColor(i,0xA0,0xA0,00);
        strip.show();
  }
  delay(500);
  pinMode(Magnetic_PIN,INPUT);
  pinMode(RECORDER_PIN,OUTPUT);
  digitalWrite(RECORDER_PIN,HIGH);
  
}

void loop() {
    if(LOW == digitalRead(Magnetic_PIN)) { // The Door is open
        if(1 == flag) {
            digitalWrite(RECORDER_PIN,LOW);
            delay(100);
            digitalWrite(RECORDER_PIN,HIGH);   
            flag = 0;
            Serial.println(" The door is open");
        } 
        for (int i = 0; i<PIXIEL_NUM; i++) {
            strip.setPixelColor(i,0x00,0xA0,00);
            strip.show();
        }        
    } else { // The Door is close
        flag = 1;
        for (int i = 0; i<PIXIEL_NUM; i++) {
            strip.setPixelColor(i,0xA0,0x00,00);
            strip.show();
        }
        
    }
    delay(100);
    Serial.println(flag);
}

void timerIsrFeedFog()
{
    wdt_reset();
    Serial.println("------------Time ISR");
}


