#include "CPipBoyDisplay.h"

PipBoyDisplay::PipBoyDisplay() {
  _tft = TFT_HX8357();
  _textSize = DEFAULT_TEXT_SIZE;
  _textColor = TFT_WHITE;
}

void PipBoyDisplay::init() {
  _tft.begin();
  _tft.setRotation(HORIZONTAL);
  _tft.fillScreen(TFT_BLACK); // Clear screen to navy background
  changeTextSize(DEFAULT_TEXT_SIZE);
  changeTextColor(YELLOW);
  moveCursor(0, 0);
  Serial.println("HERE2");

}

void PipBoyDisplay::changeTextSize(uint8_t size) {
  _textSize = size;
  _tft.setTextSize(size);
}

void PipBoyDisplay::changeTextColor(uint8_t idx) {
  _textColor = _COLORS[idx];
  _tft.setTextColor(_textColor);
}

void PipBoyDisplay::moveCursor(uint16_t x, uint16_t y) {
  _tft.setCursor(x, y);
}

void PipBoyDisplay::typeString(char* txt, bool speed) {
  auto size = strlen(txt);
  for (auto idx = 0; idx < size; idx++) {
    _tft.print(txt[idx]);
    delay(speed ? FAST_WRITE_DELAY : WRITE_DELAY);
  }
}

void PipBoyDisplay::typeStringLn(char* txt, bool speed) {
  typeString(txt, speed);
  _tft.println("");
}

void PipBoyDisplay::clear() {
  _tft.fillScreen(TFT_BLACK);
}

void PipBoyDisplay::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  _tft.drawRect(x, y, w, h, _COLORS[color]);
}

void PipBoyDisplay::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  _tft.fillRect(x, y, w, h, _COLORS[color]);
}

uint8_t PipBoyDisplay::getCursorX() {
  return _tft.getCursorX();
}

uint8_t PipBoyDisplay::getCursorY() {
  return _tft.getCursorY();
}
