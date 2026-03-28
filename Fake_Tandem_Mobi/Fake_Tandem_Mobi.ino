// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6  // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 2  // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 250  // Time (in milliseconds) to pause between pixels

void setup() {

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  rainbow();
}

void loop() {
  pumpReminder();
  delay(5000);
  allDeliveriesStoppedManually();
  delay(5000);
  pumpMalfunctionOrAlarm();
  delay(5000);
  pumpOrCGMAlert();
  delay(5000);
}

void pumpReminder() {
  //both lights blink yellow once

  pixels.clear();

  pixels.setPixelColor(0, pixels.Color(255, 255, 0));
  pixels.setPixelColor(1, pixels.Color(255, 255, 0));
  pixels.show();

  delay(DELAYVAL);

  pixels.clear();
  pixels.show();
}

void allDeliveriesStoppedManually() {
  //both lights blink white three times

  pixels.clear();

  for (int i = 0; i <= 2; i++) {

    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
    pixels.setPixelColor(1, pixels.Color(255, 255, 255));
    pixels.show();

    delay(DELAYVAL);

    pixels.clear();
    pixels.show();

    delay(DELAYVAL);
  }
}

void pumpMalfunctionOrAlarm() {
  //both lights blink red three times

  pixels.clear();

  for (int i = 0; i <= 2; i++) {

    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(1, pixels.Color(255, 0, 0));
    pixels.show();

    delay(DELAYVAL);

    pixels.clear();
    pixels.show();

    delay(DELAYVAL);
  }
}

void pumpOrCGMAlert() {
  //both lights blink yellow two times

  pixels.clear();

  for (int i = 0; i <= 1; i++) {

    pixels.setPixelColor(0, pixels.Color(255, 255, 0));
    pixels.setPixelColor(1, pixels.Color(255, 255, 0));
    pixels.show();

    delay(DELAYVAL);

    pixels.clear();
    pixels.show();

    delay(DELAYVAL);
  }
}

void rainbow() {
  pixels.clear();

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 255));
  pixels.show();

  delay(1000);
}
