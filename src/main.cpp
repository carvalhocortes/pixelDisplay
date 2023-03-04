
#include <SD.h>
#include <GifDecoder.h>
#include "FilenameFunctions.h"
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

int ledPosition(int x, int y) {
  if (x < 1 || x > WIDTH + 1) {
    Serial.println("x position invalid");
    Serial.print("x =  ");
    Serial.println(x);    
    Serial.print("The value of x must be between 1 and ");
    Serial.println(WIDTH);
    return 0;
  } 
  if (y < 1 || y > HEIGHT + 1) {
    Serial.println("y position invalid");
    Serial.print("y =  ");
    Serial.println(y);    
    Serial.print("The value of y must be between 1 and ");
    Serial.println(HEIGHT);
    return 0;
  } 
  if (y % 2 == 0){
    return ((y - 1) * WIDTH) + (x - 1);
  }
  return ((y - 1) * WIDTH) + (WIDTH - x);
}

void turnOffDisplay() {
  FastLED.clear();
  FastLED.clearData();
  FastLED.showColor(CRGB(0, 0, 0));
  FastLED.show();   
}

void testDisplay(char function[], int duration, int testBrightness) {

  FastLED.setBrightness(testBrightness);
  if (function == "pixels" || function == "all" ){
    Serial.println("Testing all pixels on display!");
    for (int y = 1; y <= HEIGHT; y++){
      for (int x = 1; x <= WIDTH; x++) {
        pixel[ledPosition(x,y)] = CRGB(255, 255, 255);
        FastLED.show();
        delay(duration / NUM_LEDS);
        turnOffDisplay();
      }
    }
  }
  if (function == "lines" || function == "all" ){
    Serial.println("Testing all lines on display!");
    for (int y = 1; y <= HEIGHT; y++){
      for (int x = 1; x <= WIDTH; x++) {
        pixel[ledPosition(x,y)] = CRGB(255, 255, 255);
      }
      FastLED.show();
      delay(duration / HEIGHT);
      turnOffDisplay();
      FastLED.show();
    }
  }
  if (function == "columns" || function == "all" ){
    Serial.println("Testing all columns on display!");
    for (int x = 1; x <= WIDTH; x++) {
      for (int y = 1; y <= HEIGHT; y++){ 
        pixel[ledPosition(x,y)] = CRGB(255, 255, 255);
      }
      FastLED.show();
      delay(duration / WIDTH);
      turnOffDisplay();
    }
  }
  if (function == "colors" || function == "all" ){
    Serial.println("Testing colors on display!");
    CRGB colors[] = {
      CRGB(255, 255, 255), //100% White
      CRGB(0, 255, 255), //100% Cyan
      CRGB(255, 255, 0), //100% Yellow
      CRGB(255, 0, 255), //100% Magenta      
      CRGB(255, 0, 0), //100% Red
      CRGB(0, 255, 0), //100% Green
      CRGB(0, 0, 255), //100% Blue
      CRGB(0, 0, 0) //100% Black
    };
    int ColorArraySize = (sizeof(colors) / 3);
    for (int i = 0; i < ColorArraySize; i++){
      FastLED.showColor(colors[i]);
      delay(duration / ColorArraySize);
      FastLED.show();
    }
  }
  if (function == "colorsBars" || function == "all" ){
    Serial.println("Testing colors bars on display!");
    CRGB colorsLineOne[] = {
      CRGB(104, 104, 104), //40% Gray
      CRGB(180, 180, 180), //75% White
      CRGB(180, 180, 16), //75% Yellow
      CRGB(16, 180, 180), //75% Cyan
      CRGB(16, 180, 16), //75% Green
      CRGB(180, 16, 180), //75% Magenta
      CRGB(180, 16, 16), //75% Red
      CRGB(16, 16, 180), //75% Blue
    };
    CRGB colorsLineTwo[] = {
      CRGB(0, 255, 255), //100% Cyan
      CRGB(16, 70, 103), //-I
      CRGB(180, 180, 180), //75% White
      CRGB(180, 180, 180), //75% White
      CRGB(180, 180, 180), //75% White
      CRGB(180, 180, 180), //75% White
      CRGB(180, 180, 180), //75% White
      CRGB(0, 0, 255), //100% Blue
    };    
    CRGB colorsLineThree[] = {
      CRGB(255, 255, 0), //100% Yellow
      CRGB(72, 16, 118), //+Q
      CRGB(0, 0, 0), //100% Black 
      CRGB(64, 64, 64), //gray
      CRGB(128, 128, 128), //gray
      CRGB(192, 192, 192), //gray
      CRGB(255, 255, 255), //100% White
      CRGB(255, 0, 0), //100% Red
    };
    CRGB colorsLineFour[] = {
      CRGB(64, 64, 64), //gray
      CRGB(0, 0, 0), //100% Black 
      CRGB(0, 0, 0), //100% Black 
      CRGB(255, 255, 255), //100% White
      CRGB(255, 255, 255), //100% White
      CRGB(0, 0, 0), //100% Black 
      CRGB(0, 0, 0), //100% Black 
      CRGB(0, 0, 0), //100% Black 
    };    
    int numberOfBars = 8;
    int proportionLineOne = 5;
    int proportionLineTwo = 1;
    int proportionLineThree = 1;
    int proportionLineFour = 1;

    int columnsSize = WIDTH / numberOfBars;
    int rowsSize = HEIGHT / (proportionLineOne + proportionLineTwo + proportionLineThree + proportionLineFour );

    int lineSizeOne = rowsSize * proportionLineOne;
    int startLineOne = 1;
    int endLineOne = startLineOne + lineSizeOne;
    int lineSizeTwo = rowsSize * proportionLineTwo;
    int startLineTwo = lineSizeOne + 1;
    int endLineTwo = startLineTwo + lineSizeTwo;
    int lineSizeThree = rowsSize * proportionLineThree;
    int startLineThree = startLineTwo + lineSizeTwo;
    int endLineThree = startLineThree + lineSizeThree;
    int lineSizeFour = rowsSize * proportionLineFour;
    int startLineFour = startLineThree + lineSizeThree;
    int endLineFour = startLineFour + lineSizeFour;
    //bar one
    for (int y = startLineOne; y < endLineOne; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[ledPosition(x,y)] = colorsLineOne[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineTwo; y < endLineTwo; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[ledPosition(x,y)] = colorsLineTwo[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineThree; y < endLineThree; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[ledPosition(x,y)] = colorsLineThree[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineFour; y < endLineFour; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[ledPosition(x,y)] = colorsLineFour[i-1]; //40% Gray;
        }
      }
    }
    FastLED.show();
    delay(duration);
  }
  turnOffDisplay();
}

void screenClearCallback(void) {
  turnOffDisplay();
}

void updateScreenCallback(void) {
  FastLED.show();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  pixel[ledPosition(x+1,y+1)] = CRGB(red, green, blue);
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
