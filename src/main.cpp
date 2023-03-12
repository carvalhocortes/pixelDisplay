
#include <SD.h>
#include <GifDecoder.h>
#include "filenameFunctions.h"
#include "fastLedToXYcord.h"
#include <FastLED.h>

#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   100
#define WIDTH 16
#define HEIGHT 16
#define DATA_PIN 2 // Pin D4 do ESP8266
#define SD_CS 0 // pin D3 do ESP8266
#define GIF_DIRECTORY "/gifs"

const int NUM_LEDS = WIDTH * HEIGHT;
float brightness = 50;

// Define the array of leds
CRGB pixel[NUM_LEDS];
GifDecoder<WIDTH, HEIGHT, 12> decoder;
int num_files;

void screenClearCallback(void) {
  turnOffDisplay();
}

void updateScreenCallback(void) {
  FastLED.show();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  pixel[getPositionByXYCoordinates(x+1, y+1, WIDTH, HEIGHT)] = CRGB(red, green, blue);
}

void setup() {
  decoder.setScreenClearCallback(screenClearCallback);
  decoder.setUpdateScreenCallback(updateScreenCallback);
  decoder.setDrawPixelCallback(drawPixelCallback);

  decoder.setFileSeekCallback(fileSeekCallback);
  decoder.setFilePositionCallback(filePositionCallback);
  decoder.setFileReadCallback(fileReadCallback);
  decoder.setFileReadBlockCallback(fileReadBlockCallback);

  decoder.setFileSizeCallback(fileSizeCallback);

  Serial.begin(115200);
  Serial.println("Serial started!");
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixel, NUM_LEDS);
  turnOffDisplay();
  testDisplay("colorsBars", 5000, 64);
  FastLED.setBrightness(brightness);

  Serial.println("Starting AnimatedGIFs Sketch");

  if(initFileSystem(SD_CS) < 0) {
    Serial.println("No SD card");
    while(1);
  }

  num_files = enumerateGIFFiles(GIF_DIRECTORY, true);

  if(num_files < 0) {
    Serial.println("No gifs directory");
    while(1);
  }

  if(!num_files) {
    Serial.println("Empty gifs directory");
    while(1);
  }
}

void loop() {
  static unsigned long displayStartTime_millis;
  static bool playNextGif = true;

  static uint32_t lastFrameDisplayTime = 0;
  static unsigned int currentFrameDelay = 0;

  static int index = 0;

  unsigned long now = millis();

#if 1
  if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) || decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
    playNextGif = true;
#else
  if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) && decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
    playNextGif = true;
#endif

  if((millis() - lastFrameDisplayTime) > currentFrameDelay) {
    if(playNextGif){
      playNextGif = false;
      if (openGifFilenameByIndex(GIF_DIRECTORY, index) >= 0) {
        if(decoder.startDecoding() < 0) {
          playNextGif = true;
          return;
        }
        displayStartTime_millis = now;
      }
      if (++index >= num_files) {
        index = 0;
      }
    }
    int result = decoder.decodeFrame(false);
    lastFrameDisplayTime = now;
    currentFrameDelay = decoder.getFrameDelay_ms();
    if(result < 0) {
      playNextGif = true;
      currentFrameDelay = 0;
    }
  }
}
