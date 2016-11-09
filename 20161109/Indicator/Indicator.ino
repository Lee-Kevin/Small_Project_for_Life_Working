/*
 * File: Humidifier_Arduino.ino
 * 
 * Author: Jiankai Li
 * Date: 2016-11-09
 */
#include <Process.h>
#include <string.h>
#include <FileIO.h>
#include <avr/wdt.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>



#define PIXIEL_PIN 4
#define PIXIEL_NUM 30

#define INTERVAL      1          // Time interval unit s
#define INTERVAL_DATA 5          // Time interval unit s
#define ATOMIZER_PIN     5          // 
#define BUTTON_PIN       4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXIEL_NUM, PIXIEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousTime = 0;  // define the last time  unit s
unsigned long previousTimeData = 0;  // define the data last time  unit 

uint8_t command;

void setup() {
  Serial.begin(9600);
  Serial.println("power on!");

  // Initialize Bridge
  Bridge.begin();
  // Initialize the FileSystem

  previousTime = millis()/1000;
  previousTimeData = millis()/1000 - random(3,10);
  
  delay(1000);
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
}

void loop() {
	unsigned long currentTime = millis()/1000;

	if (currentTime - previousTime >= INTERVAL) {

		previousTime = currentTime;
        Serial.println("bridgeGetAtomizerCom");
		command = bridgeGetCommand();
		if (1 == command) {
			Serial.println("The account is online");
            for (int i = 0; i<PIXIEL_NUM; i++) {
                strip.setPixelColor(i,0xA0,00,00);
                strip.show();
            }
		} else if (0 == command) {
            Serial.println("The account is offline");
            for (int i = 0; i<PIXIEL_NUM; i++) {
                strip.setPixelColor(i,0x00,0xA0,00);
                strip.show();
            }
		} else {
            Serial.println("Error");
            for (int i = 0; i<PIXIEL_NUM; i++) {
                strip.setPixelColor(i,0x00,0x00,0xA0);
                strip.show();
            }
        }
	}

}


/* Get the relay command from 7688 */
uint8_t bridgeGetCommand() {
	Process p;
	String str;
	uint8_t command;
	p.begin("python");
	p.addParameter("/root/account.py");
	p.run();
	while (p.available() > 0) {
		char c = p.read();
		str += c;
	}
	command = str.toInt();
	return command;
	
}
void timerIsrFeedFog()
{
    wdt_reset();
    Serial.println("------------Time ISR");
}


