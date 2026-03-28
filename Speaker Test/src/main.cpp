#include "Arduino.h"
#include "Audio.h"
#include "SPIFFS.h"

// XIAO S3 to MAX98357A Pin Map
#define I2S_LRC   7 
#define I2S_BCLK  8 
#define I2S_DOUT  9 

Audio audio;

void setup() {
    Serial.begin(115200);
  
    
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); // Start at mid-volume to save power

    Serial.println("Starting playback...");
    audio.connecttoFS(SPIFFS, "/Music.mp3");
}

void loop() {
    audio.loop();
}