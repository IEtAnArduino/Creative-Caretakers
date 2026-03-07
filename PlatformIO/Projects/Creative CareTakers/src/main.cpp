// Test
// Copy and pasted example code
#include "Arduino.h"
#include <Wire.h>
#include <Audio.h>
#include "Adafruit_TCS34725.h"
#include "SPIFFS.h"


#define I2S_DOUT  27
#define I2S_BCLK  26
#define I2S_LRC   25

Audio audio;
/*
// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true
*/

// Mammraj, test the colors of each light and add values here
// Right now, pure red, green, and blue are here as placeholders
byte signalcolors[][3] = {
  {210, 25, 39},
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255}
};

// our RGB -> eye-recognized gamma color
byte gammatable[256];
bool isYellow = false;
int whiteCount = 0;  
bool isWhite = false;
int redCount = 0;    
bool isRed = false;

//Defines 2 RGB Sensors
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Define the two separate I2C buses
TwoWire I2Cone = Wire;
TwoWire I2Ctwo = TwoWire(1);

void check_a_signals(float c1[3], float c2[3])

  {
    bool bothSeeYellow = ( (c1[0] > 150 && c1[1] > 150 && c1[2] < 40) && (c2[0] > 150 && c2[1] > 150 && c2[2] < 40) );
    bool bothSeeWhite  = ( (c1[0] > 200 && c1[1] > 200 && c1[2] > 200) && (c2[0] > 200 && c2[1] > 200 && c2[2] > 200) );
    bool bothSeeRed    = ( (c1[0] > 150 && c1[1] < 40 && c1[2] < 40) && (c2[0] > 150 && c2[1] < 40 && c2[2] < 40) );
    //Yellow Check
    if (bothSeeYellow && !isYellow) 
      {
        isYellow = true;
        Serial.println("CONFIRMED: Both sensors see Yellow!");
        audio.connecttoFS(SPIFFS, "/Pump_Reminder.mp3");
      }
    
  else if (!bothSeeYellow) 
    {
      isYellow = false; 
    }

    //White Check
    if (bothSeeWhite && !isWhite) 
      {
        whiteCount++;
        isWhite = true;
        Serial.println("CONFIRMED: Both sensors see White!");
        
      }
      if (whiteCount >= 3) 
      {
        audio.connecttoFS(SPIFFS, "/Deliveries_Stopped_Manually.mp3");
        whiteCount = 0;
      }
    
  else if (!bothSeeWhite) 
    {
      isWhite = false; 
    }

    //Red Check
    if (bothSeeRed && !isRed) 
      {
        redCount++;
        isRed = true;
        Serial.println("CONFIRMED: Both sensors see Red!");
        
      }
      if (redCount >= 3) 
      {
        audio.connecttoFS(SPIFFS, "/Pump_Malfunction_or_Alarm.mp3");
        redCount = 0;
      }
    
  else if (!bothSeeRed) 
    {
      isRed = false; 
    }

  }




{
  int tolerance = 5;

  for(int i = 0; i < sizeof(signalcolors) / sizeof(signalcolors[0]); i++){
    // messy nested if statements here to check if each RGB value is within an acceptable range
    // tolerance arbitrarily set to +-5 around the "nominal" color value
    if(colors[0] > signalcolors[i][0] - tolerance && colors[0] < signalcolors[i][0] + tolerance){
      if(colors[1] > signalcolors[i][1] - tolerance && colors[1] < signalcolors[i][1] + tolerance){
        if(colors[2] > signalcolors[i][2] - tolerance && colors[2] < signalcolors[i][2] + tolerance){
          Serial.println("Tandem Mobi light signal detected!");
          
        }
      }
    }
  }
}



void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
  //Serial.println("Creative Caretakers' Glucose Monitor to Audio Warning System (Come up with a better name later!!!)");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15); // Range 0-21
    I2Cone.begin(21, 22); 
    I2Ctwo.begin(17, 16);
  if (tcs1.begin(0x29, &I2Cone) && tcs2.begin(0x29, &I2Ctwo)) 
  {
    //Plays "Found Sensor" through speaker
    audio.connecttoFS(SPIFFS, "/Found.mp3");
    Serial.println("Found sensor");
  } else {
    //Plays "No TCS34725 found ... check your connections" through speaker
    audio.connecttoFS(SPIFFS, "/No_Found.mp3");
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;  // halt!
  }


  /*
  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }*/
}


void loop() {
  audio.loop();

  float c1[3], c2[3];

    tcs1.setInterrupt(false);
    tcs2.setInterrupt(false);
    
    delay(60); 

    tcs1.getRGB(&c1[0], &c1[1], &c1[2]);
    tcs2.getRGB(&c2[0], &c2[1], &c2[2]);

    tcs1.setInterrupt(true);
    tcs2.setInterrupt(true);

    check_a_signals(c1, c2);

  Serial.print("R:\t");
  Serial.print(int(colors[0]));
  Serial.print("\tG:\t");
  Serial.print(int(colors[1]));
  Serial.print("\tB:\t");
  Serial.print(int(colors[2]));

  //  Serial.print("\t");
  //  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

  


  /*
  analogWrite(redpin, gammatable[(int)red]);
  analogWrite(greenpin, gammatable[(int)green]);
  analogWrite(bluepin, gammatable[(int)blue]);
*/
}
