// Test
// Copy and pasted example code

#include <Wire.h>
#include "Adafruit_TCS34725.h"

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


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void checksignals(float colors[3]) {
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
  //Serial.println("Creative Caretakers' Glucose Monitor to Audio Warning System (Come up with a better name later!!!)");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
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
  float colors[3];

  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&colors[0], &colors[1], &colors[2]);

  tcs.setInterrupt(true);  // turn off LED DOES NOT WORK!!!!

  Serial.print("R:\t");
  Serial.print(int(colors[0]));
  Serial.print("\tG:\t");
  Serial.print(int(colors[1]));
  Serial.print("\tB:\t");
  Serial.print(int(colors[2]));

  //  Serial.print("\t");
  //  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

  checksignals(colors);


  /*
  analogWrite(redpin, gammatable[(int)red]);
  analogWrite(greenpin, gammatable[(int)green]);
  analogWrite(bluepin, gammatable[(int)blue]);
*/
}
