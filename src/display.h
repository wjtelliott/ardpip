#include "Adafruit_ILI9341.h"
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define HORIZONTAL 1
#define DEFAULT_TEXT_SIZE 1
#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define BLACK 4
#define WRITE_DELAY 30
#define FAST_WRITE_DELAY 0
#define COLORS_SIZE sizeof(COLORS)/sizeof(COLORS[0])
const uint16_t COLORS[] = {
  ILI9341_RED,
  ILI9341_BLUE,
  ILI9341_GREEN,
  ILI9341_YELLOW,
  ILI9341_BLACK
};

void changeDisplayTextSize(uint8_t size) {
  tft.setTextSize(size);
}

void moveDisplayCursor(uint16_t x, uint16_t y) {
  tft.setCursor(x, y);
}

void changeDisplayColor(uint8_t idx) {
  tft.setTextColor(COLORS[idx]);
}

void initializeDisplay() {
  debug("ili9341", "setting up display...");
  tft.begin();
  tft.setRotation(HORIZONTAL);
  changeDisplayTextSize(DEFAULT_TEXT_SIZE);
  changeDisplayColor(YELLOW);
  moveDisplayCursor(0, 0);
}

void typeStringOnDisplay(char* txt, bool speed) {
  auto size = strlen(txt);
  for (auto idx = 0; idx < size; idx++) {
    tft.print(txt[idx]);
    delay(speed ? FAST_WRITE_DELAY : WRITE_DELAY);
  }
}

void typeStringLnOnDisplay(char* txt, bool speed) {
  typeStringOnDisplay(txt, speed);
  tft.println("");
}

void clearDisplay() {
  tft.fillScreen(ILI9341_BLACK);
}

uint8_t getCursorX() {
  return tft.getCursorX();
}

uint8_t getCursorY() {
  return tft.getCursorY();
}

void fillRectOnDisplay(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) {
  tft.fillRect(x, y, width, height, COLORS[color]);
}