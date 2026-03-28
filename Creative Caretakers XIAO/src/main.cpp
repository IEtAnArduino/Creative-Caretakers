#include "Arduino.h"
#include <Wire.h>
#include <Audio.h>
#include "Adafruit_TCS34725.h"
#include "SPIFFS.h"

#define I2S_DOUT  42
#define I2S_BCLK  2
#define I2S_LRC   1

Audio audio;

byte signalcolors[][3] = {
  {210, 25, 39},
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255}
};

byte gammatable[256];
bool isYellow = false;
int whiteCount = 0;  
bool isWhite = false;
int redCount = 0;    
bool isRed = false;

Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

TwoWire I2Cone = Wire;
TwoWire I2Ctwo = TwoWire(1);

void check_a_signals(float c1[3], float c2[3]) {
  bool bothSeeYellow = ( (c1[0] > 150 && c1[1] > 150 && c1[2] < 40) && (c2[0] > 150 && c2[1] > 150 && c2[2] < 40) );
  bool bothSeeWhite  = ( (c1[0] > 200 && c1[1] > 200 && c1[2] > 200) && (c2[0] > 200 && c2[1] > 200 && c2[2] > 200) );
  bool bothSeeRed    = ( (c1[0] > 150 && c1[1] < 40 && c1[2] < 40) && (c2[0] > 150 && c2[1] < 40 && c2[2] < 40) );

  if (bothSeeYellow && !isYellow) {
    isYellow = true;
    Serial.println("CONFIRMED: Both sensors see Yellow!");
    audio.connecttoFS(SPIFFS, "/Pump_Reminder.mp3");
  } else if (!bothSeeYellow) {
    isYellow = false; 
  }

  if (bothSeeWhite && !isWhite) {
    whiteCount++;
    isWhite = true;
    Serial.println("CONFIRMED: Both sensors see White!");
  }
  if (whiteCount >= 3) {
    audio.connecttoFS(SPIFFS, "/Deliveries_Stopped_Manually.mp3");
    whiteCount = 0;
  } else if (!bothSeeWhite) {
    isWhite = false; 
  }

  if (bothSeeRed && !isRed) {
    redCount++;
    isRed = true;
    Serial.println("CONFIRMED: Both sensors see Red!");
  }
  if (redCount >= 3) {
    audio.connecttoFS(SPIFFS, "/Pump_Malfunction_or_Alarm.mp3");
    redCount = 0;
  } else if (!bothSeeRed) {
    isRed = false; 
  }
}

// Fixed the floating bracket problem by wrapping this in a function
void check_tolerances(float colors[3]) {
  int tolerance = 5;
  for(int i = 0; i < sizeof(signalcolors) / sizeof(signalcolors[0]); i++) {
    if(colors[0] > signalcolors[i][0] - tolerance && colors[0] < signalcolors[i][0] + tolerance) {
      if(colors[1] > signalcolors[i][1] - tolerance && colors[1] < signalcolors[i][1] + tolerance) {
        if(colors[2] > signalcolors[i][2] - tolerance && colors[2] < signalcolors[i][2] + tolerance) {
          Serial.println("Tandem Mobi light signal detected!");
        }
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15);
  I2Cone.begin(21, 22); 
  I2Ctwo.begin(17, 16);

  if (tcs1.begin(0x29, &I2Cone) && tcs2.begin(0x29, &I2Ctwo)) {
    audio.connecttoFS(SPIFFS, "/Found.mp3");
    Serial.println("Found sensor");
  } else {
    audio.connecttoFS(SPIFFS, "/No_Found.mp3");
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  audio.loop();

  float c1[3], c2[3];

  tcs1.setInterrupt(false);
  tcs2.setInterrupt(false);
  delay(10); 

  tcs1.getRGB(&c1[0], &c1[1], &c1[2]);
  tcs2.getRGB(&c2[0], &c2[1], &c2[2]);

  tcs1.setInterrupt(true);
  tcs2.setInterrupt(true);

  check_a_signals(c1, c2);
  check_tolerances(c1); // Now calling the fixed function

  Serial.print("R:\t");
  Serial.print(int(c1[0]));
  Serial.print("\tG:\t");
  Serial.print(int(c1[1]));
  Serial.print("\tB:\t");
  Serial.print(int(c1[2]));
  Serial.print("\n");
}