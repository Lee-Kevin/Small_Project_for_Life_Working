/*
 * Arduino Music.h
 * This is a music player game. 
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jiankai Li
 * Created Time: Jan 2016
 * Modified Time:
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include "pitches.h"
#include <SoftwareSerial.h>
#include "LEDMatrix.h"

#define TimeInterval  3000 // unit ms

#define RECEIVE_PIN      2
#define TRANSMIT_PIN     3

#define BUTTON_LEFT      5
#define BUTTON_RIGHT     6

#define BUZZER           8

enum status {
    GameStart = 0,
    GameStop  = 2,
    GameSuccess = 3,
    Idle     = 4,
};

typedef enum status estatus_t;
estatus_t WorkingStatus;

/*
enum gamut_type_t // define notes
{ 
	NOTE_R0 = 0, // Null
	
	NOTE_C4 = 1, // Do
	NOTE_D4 = 2, // Re
	NOTE_E4 = 3, // Mi
	NOTE_F4 = 4, // Fa
	NOTE_G4 = 5, // So
	NOTE_A4 = 6, // La
	NOTE_B4 = 7, // Si
	
	NOTE_C5 = 8, // Do
	NOTE_D5 = 9, // Re
	NOTE_E5 = 10, // Mi
	NOTE_F5 = 11, // Fa
	NOTE_G5 = 12, // So
	NOTE_A5 = 13, // La
	NOTE_B5 = 14, // Si
	
	NOTE_C6 = 15, // Do
	NOTE_D6 = 16, // Re
	NOTE_E6 = 17, // Mi
	NOTE_F6 = 18, // Fa
	NOTE_G6 = 19, // So
	NOTE_A6 = 20, // La
	NOTE_B6 = 21, // Si
};

*/

uint16_t MusicType[][2] = {
    {0,0},{1,NOTE_C4},{2,NOTE_C4},{3,NOTE_E4},{4,NOTE_F4},{5,NOTE_G4},{6,NOTE_A4},{7,NOTE_B4},
    {8,NOTE_C5},{9,NOTE_D5},{10,NOTE_E5},{11,NOTE_F5},{12,NOTE_G5},{13,NOTE_A5},{14,NOTE_B5},
    {15,NOTE_C6},{16,NOTE_D6},{17,NOTE_E6},{18,NOTE_F6},{19,NOTE_G6},{20,NOTE_A6},{21,NOTE_B6},
};


uint16_t beat_time[8][2] = {
    {0,500},{1,1000},{2,2000},{3,4000},{4,250},{5,125},{6,62},{7,31},
};



uint8_t happybirthday_song[][2] = {
    {5,4},
    {5,4},
    {6,0},
    {5,0},
    {8,0},
    {7,1},
    
    {5,4},
    {5,4},
    {6,0},
    {5,0},    
    {9,0},   
    {8,1},
    
    {5,4},
    {5,4},  
    {12,0},
    {10,0},
    {8,0},
    {7,0},
    {6,0},
    
    {11,4},
    {11,5},
    {10,0},
    {8,0},
    {9,0},
    {8,1},
};

uint8_t seeed_logo32_16[64] = {
0x00,0x04,0x20,0x00,0x00,0x0C,0x30,0x00,0x00,0x08,0x30,0x00,0x00,0x18,0x18,0x00,
0x00,0x18,0x18,0x00,0x02,0x18,0x18,0x00,0x03,0x18,0x18,0x80,0x03,0x18,0x18,0x80,
0x03,0x18,0x19,0x80,0x01,0x98,0x19,0x80,0x01,0x98,0x33,0x80,0x00,0xCC,0x33,0x00,
0x00,0xE4,0x26,0x00,0x00,0x76,0x4E,0x00,0x00,0x38,0x1C,0x00,0x00,0x0C,0x70,0x00,
};

uint8_t arrow[6][64] = {   
    {0x00,0x0E,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,0x07,0xC0,0x00,
    0x00,0x01,0xE0,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x7C,0x00,
    0x00,0x00,0x7C,0x00,0x00,0x00,0xF8,0x00,0x00,0x01,0xF0,0x00,0x00,0x03,0xE0,0x00,
    0x00,0x07,0xC0,0x00,0x00,0x0F,0x80,0x00,0x00,0x0F,0x00,0x00,0x00,0x0E,0x00,0x00,}, // right 1
    
    {0x03,0xE1,0xF0,0x00,0x03,0xF1,0xF8,0x00,0x01,0xF0,0x7C,0x00,0x00,0xF8,0x3F,0x00,
    0x00,0x7C,0x1F,0x80,0x00,0x1E,0x07,0xC0,0x00,0x1F,0x03,0xE0,0x00,0x0F,0x07,0xE0,
    0x00,0x0F,0x0F,0xC0,0x00,0x3E,0x1F,0xC0,0x00,0x7C,0x3F,0x00,0x00,0x78,0x3C,0x00,
    0x00,0xF0,0x78,0x00,0x01,0xF0,0xF0,0x00,0x03,0xE1,0xF0,0x00,0x03,0xC3,0xE0,0x00,},  // right 2
    
    {0x0C,0x0E,0x0F,0x00,0x1E,0x0F,0x07,0x80,0x0F,0x0F,0x87,0xC0,0x07,0x87,0xC1,0xE0,
    0x07,0xC1,0xE0,0xF0,0x03,0xE0,0xF0,0xF0,0x01,0xF8,0xF8,0x78,0x00,0xF8,0x7C,0x3C,
    0x00,0xF8,0x7C,0x3E,0x01,0xF8,0xF8,0x7C,0x01,0xF1,0xF0,0xF8,0x03,0xC3,0xE1,0xF0,
    0x07,0x87,0xC3,0xE0,0x0F,0x0F,0x83,0xC0,0x1E,0x0F,0x07,0x80,0x1C,0x0E,0x0F,0x00,},  //right 3
    
    {0x00,0x00,0x38,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0xFC,0x00,0x00,0x01,0xF8,0x00,
    0x00,0x03,0xE0,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0x80,0x00,0x00,0x1F,0x00,0x00,
    0x00,0x0F,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,0x07,0xC0,0x00,0x00,0x03,0xE0,0x00,
    0x00,0x01,0xF0,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x3C,0x00,},  // left 1
    
    {0x00,0x0F,0x03,0xC0,0x00,0x1F,0x07,0xC0,0x00,0x1E,0x0F,0x80,0x00,0x7C,0x1F,0x00,
    0x00,0xFC,0x3E,0x00,0x00,0xF8,0x3C,0x00,0x01,0xF0,0x78,0x00,0x03,0xE0,0xF0,0x00,
    0x03,0xE0,0xF0,0x00,0x01,0xF0,0x78,0x00,0x00,0xF8,0x3E,0x00,0x00,0x7C,0x1F,0x00,
    0x00,0x3E,0x0F,0x80,0x00,0x1F,0x07,0xC0,0x00,0x0F,0x83,0xE0,0x00,0x03,0x81,0xC0,},  // left 2
    
    {0x00,0x7C,0x38,0x0E,0x00,0xF8,0x7C,0x3E,0x01,0xF0,0xFC,0x7E,0x03,0xE1,0xF8,0xF8,
    0x07,0xC3,0xE1,0xF0,0x0F,0x87,0xC3,0xE0,0x1F,0x0F,0x87,0xC0,0x1E,0x1F,0x0F,0x80,
    0x1E,0x0F,0x0F,0x80,0x1F,0x0F,0x87,0x80,0x0F,0x87,0xC3,0xC0,0x07,0xC3,0xE1,0xE0,
    0x03,0xF1,0xF0,0xF0,0x01,0xF8,0xF8,0x78,0x00,0xFC,0x7C,0x3C,0x00,0x7C,0x3C,0x1E,},  // left 3
    
   // {0x00,0x00,0x00,0x00,0x00}, //blank
};

SoftwareSerial matrix_serial(RECEIVE_PIN, TRANSMIT_PIN);

LEDMatrix led;

uint32_t last_millis,display_millis;
uint8_t random_index, button_status,score,temptime;
uint16_t myindex;
void setup()
{
	Serial.begin(9600);	


    WorkingStatus = Idle;
    
    score=66;
    
    last_millis = millis();
    display_millis = last_millis-5000;
    
    pinMode(BUTTON_LEFT,INPUT);
    pinMode(BUTTON_RIGHT,INPUT);
    
    Serial.println("System Init");
    led.clear();                            // clear screen
    led.drawImage(32, 0, 32, 16, seeed_logo32_16, GREEN);       // draw image. This example provides an example image data. You can use your own image data. Note: Max:64*64!

    // clear screen
    // led.displayLongInteger(40, 0, 3, FONT_12X16, RED);
    // delay(2000);
    
}

void loop()
{
    switch(WorkingStatus) {
    case GameStart:
        Serial.println("GAME Start: ");
        myindex = 0;
        
        for (int i=3; i>0; i--) {
            //char temp = i-'0';
            led.clear();                            // clear screen
            led.displayLongInteger(42, 0, long(i), FONT_12X16, RED);
            delay(1000);
        }
        
        
        last_millis = millis();
        for(int i=1;i<=sizeof(happybirthday_song)/2;) {
            if (millis() - display_millis > beat_time[(happybirthday_song[i-1][1])][1] +100) {
                random_index = random(0,6);
                led.clear();                            // clear screen
                
                // delay(5);
                
                led.drawImage(32, 0, 32, 16, arrow[random_index], GREEN);  
                display_millis = millis();
            }
            uint8_t status = getButtonStatus();
            delay(10);
            status = getButtonStatus();

            if (((status-1) == random_index/3) && status != 0 && status != 3) {
                tone(BUZZER,MusicType[(happybirthday_song[i-1][0])][1],beat_time[(happybirthday_song[i-1][1])][1]);
                delay(beat_time[(happybirthday_song[i-1][1])][1]-10);
                i++;
            }
            delay(10);
            
            }
        
        delay(1000);
        // 11625
        // score = (100-myindex + sizeof(happybirthday_song)/2 < 0) ? 0 : (100-myindex + sizeof(happybirthday_song)/2);
        temptime = ((millis()-last_millis)/1000>30) ? 30 : (millis()-last_millis)/1000;
        score = map(temptime-12,0,30,100,0);
        score>=80? WorkingStatus = GameSuccess : WorkingStatus = GameStop;
        
        Serial.print("score: ");
        Serial.println(score);
        break;
    case GameStop:
        if (millis() - last_millis > TimeInterval) {

            led.clear();                            // clear screen
            led.displayLongInteger(42,16,score,FONT_6X8,RED);
            delay(10);
            led.displayString(36,8,"FAIL",6,FONT_6X8,RED);
            
            
        }
        last_millis = millis();
        button_status = getButtonStatus();
        delay(10);
        button_status = getButtonStatus();
        if (button_status == 3) {
            WorkingStatus = GameStart;
        }
        break;
    case GameSuccess:
        if (millis() - last_millis > TimeInterval) {
            // char str[14];
            // sprintf(str," %d GAME OVER",score);
            
            led.clear();                            // clear screen
            led.displayLongInteger(42, 16, score, FONT_6X8, RED);
            led.displayString(39,8,"WIN",3,FONT_6X8,RED);
            
            
        }
        last_millis = millis();
        button_status = getButtonStatus();
        delay(10);
        button_status = getButtonStatus();
        if (button_status == 3) {
            WorkingStatus = GameStart;
        }
        break;
    case Idle:
        button_status = getButtonStatus();
        Serial.print("Button Status: ");
        Serial.println(button_status);
        if(button_status == 3) {
            WorkingStatus = GameStart;
        }
        if (millis() - last_millis > TimeInterval+5000) {
            last_millis = millis();
            led.drawImage(32, 0, 32, 16, seeed_logo32_16, GREEN);       // draw image. This example provides an example image data. You can use your own image data. Note: Max:64*64!
        }
        
        break;
    default:
        WorkingStatus = Idle;
        break;
    }

}

uint8_t getButtonStatus() {
    uint8_t buttonleft, buttonright,status;
    buttonleft = digitalRead(BUTTON_LEFT);
    buttonright = digitalRead(BUTTON_RIGHT);
    if(buttonright == HIGH && buttonleft == HIGH) {
        return 3;
    } else if(buttonleft == HIGH) {
        return 2;
    } else if (buttonright == HIGH) {
        return 1;
    } else {
        return 0;
    }
    
}

