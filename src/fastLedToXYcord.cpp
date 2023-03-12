#include "fastLedToXYcord.h"

#include <FastLED.h>
#include <HardwareSerial.h>

void turnOffDisplay() {
  FastLED.clear();
  FastLED.clearData();
  FastLED.showColor(CRGB(0, 0, 0));
  FastLED.show();
}

int getPositionByXYCoordinates( int x, int y, int width, int height) {
  if (x < 1 || x > width + 1) {
    Serial.println("x position invalid");
    Serial.print("x =  ");
    Serial.println(x);
    Serial.print("The value of x must be between 1 and ");
    Serial.println(width);
    return 0;
  }
  if (y < 1 || y > height + 1) {
    Serial.println("y position invalid");
    Serial.print("y =  ");
    Serial.println(y);
    Serial.print("The value of y must be between 1 and ");
    Serial.println(height);
    return 0;
  }
  if (y % 2 == 0){
    return ((y - 1) * width) + (x - 1);
  }
  return ((y - 1) * width) + (width - x);
}

void testDisplay(char function[], int duration, int testBrightness) {

  FastLED.setBrightness(testBrightness);
  if (function == "pixels" || function == "all" ){
    Serial.println("Testing all pixels on display!");
    for (int y = 1; y <= HEIGHT; y++){
      for (int x = 1; x <= WIDTH; x++) {
        pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = CRGB(255, 255, 255);
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
        pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = CRGB(255, 255, 255);
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
        pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = CRGB(255, 255, 255);
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
          pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = colorsLineOne[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineTwo; y < endLineTwo; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = colorsLineTwo[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineThree; y < endLineThree; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = colorsLineThree[i-1]; //40% Gray;
        }
      }
    }
    for (int y = startLineFour; y < endLineFour; y++){
      for (int i = 1; i <= numberOfBars; i++){
        for (int x = (1 + (i - 1) * columnsSize); x <= (columnsSize * i); x++) {
          pixel[getPositionByXYCoordinates(x, y, WIDTH, HEIGHT)] = colorsLineFour[i-1]; //40% Gray;
        }
      }
    }
    FastLED.show();
    delay(duration);
  }
  turnOffDisplay();
}
