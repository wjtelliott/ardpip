/***************************************************************************************
** File Name  :         CPipBoyDisplay.cpp
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         Implementation of our display interface
***************************************************************************************/

#include "CPipBoyDisplay.h"
#include "CDrawBMP.h"
#include "CPipBoyPageNames.h"

PipBoyDisplay::PipBoyDisplay() {
  _tft = TFT_HX8357();
}

void PipBoyDisplay::init() {
  _tft.init();
  _tft.begin();
  _tft.setRotation(HORIZONTAL);
  _tft.fillScreen(TFT_BLACK); // Clear screen to navy background
  changeTextSize(DEFAULT_TEXT_SIZE);
  changeTextColor(GREEN);
  moveCursor(0, 0);

  if (!SD.begin(SDC_CS)) {
    Serial.println("SD CARD READ FAILED");
    for(;;);
  }
}

void PipBoyDisplay::changeTextSize(uint8_t size) {
  _tft.setTextSize(size);
}

void PipBoyDisplay::changeTextColor(uint8_t idx) {
  _tft.setTextColor(_COLORS[idx]);
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

void PipBoyDisplay::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  // color is unused rn
  _tft.drawLine(x1, y1, x2, y2, TFT_GREEN);
}

void PipBoyDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _tft.drawRect(x, y, w, h, _COLORS[color]);
}

void PipBoyDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  _tft.fillRect(x, y, w, h, _COLORS[color]);
}

void PipBoyDisplay::displayError(char * msg) {
  typeStringLn(msg, true);
  for(;;);
}

void PipBoyDisplay::drawSDImage(char* filename) {
  drawBMP(filename, 0, 0, false, _tft);
}

void PipBoyDisplay::drawImage(uint16_t x, uint16_t y, uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t color) {
  _tft.drawBitmap(x, y, bitmap, width, height, color);
}

void PipBoyDisplay::drawHudTopLabel() {
  drawLine(10, 40, 10, 20, 0);
  drawLine(10, 20, 460, 20, 0);
  drawLine(460, 20, 460, 40, 0);
}
void PipBoyDisplay::clearTopHud() {
  fillRect(10, 20, 460, 35, 4);
}

void PipBoyDisplay::drawHudBottomLabel() {
  drawLine(10, 300, 10, 280, 0);
  drawLine(10, 300, 460, 300, 0);
  drawLine(460, 300, 460, 280, 0);
}

void PipBoyDisplay::clearBottomHud() {
  fillRect(10, 280, 460, 35, 4);
}

void PipBoyDisplay::drawTopHud(uint8_t page) {
  switch (page) {
    default:
    case 0:
      drawStatPageTopHud();
      break;
    case 1:
      drawItemPageTopHud();
      break;
    case 2:
      break;
  }
}

void PipBoyDisplay::drawBottomHud(uint8_t page, uint8_t category) {
  switch (page) {
    default:
    case 0:
      drawStatPageBottomHud(category);
      break;
    case 1:
      drawItemPageBottomHud(category);
      break;
    case 2:
      break;
  }
}

void PipBoyDisplay::drawStatPageTopHud() {
  drawHudTopLabel();

  fillRect(20, 15, 80, 10, 4); // delete drawn line
  moveCursor(30, 15);
  typeString(STAT_PAGE, true);

  drawLine(205, 20, 205, 40, 0);
  fillRect(206, 20, 5, 5, 4);
  moveCursor(95, 25);
  typeString("  LVL   8", true);

  drawLine(325, 20, 325, 40, 0);
  fillRect(326, 20, 5, 5, 4);
  moveCursor(220, 25);
  typeString("HP 55/96", true);

  moveCursor(335, 25);
  typeString("XP 542/850", true);
}

void PipBoyDisplay::drawItemPageTopHud() {
  drawHudTopLabel();

  // text
  fillRect(20, 15, 80, 10, 4); // delete drawn line
  moveCursor(30, 15);
  typeString(INVENTORY_PAGE, true);

  // wg
  drawLine(205, 20, 205, 40, 0);
  fillRect(206, 20, 5, 5, 4);
  moveCursor(95, 25);
  typeString("Wg 88/190", true);

  //hp
  drawLine(325, 20, 325, 40, 0);
  fillRect(326, 20, 5, 5, 4);
  moveCursor(220, 25);
  typeString("HP 55/96", true);

  //caps
  moveCursor(335, 25);
  typeString("Caps 33521", true);
}

void PipBoyDisplay::drawItemPageBottomHud(uint8_t selected) {
  drawHudBottomLabel();

  fillRect(20, 300, 100, 5, 4);
  moveCursor(30,295);
  typeString(INVENTORY_CATEGORY1, true);

  fillRect(130, 300, 100, 5, 4);
  moveCursor(140,295);
  typeString(INVENTORY_CATEGORY2, true);

  fillRect(240, 300, 55, 5, 4);
  moveCursor(250,295);
  typeString(INVENTORY_CATEGORY3, true);

  fillRect(305, 300, 65, 5, 4);
  moveCursor(315,295);
  typeString(INVENTORY_CATEGORY4, true);

  fillRect(380, 300, 65, 5, 4);
  moveCursor(390,295);
  typeString(INVENTORY_CATEGORY5, true);

  int16_t x = 0;
  int16_t y = 292;
  int16_t width = 1;
  int16_t height = 21;
  switch (selected) {
    default:
    case 0:
      x = 20;
      width = 100;
      break;
    case 1:
      x = 130;
      width = 100;
      break;
    case 2:
      x = 240;
      width = 55;
      break;
    case 3:
      x = 305;
      width = 65;
      break;
    case 4:
      x = 380;
      width = 65;
      break;
  }
  drawRect(x, y, width, height, 2);
}

void PipBoyDisplay::drawStatPageBottomHud(uint8_t selected) {
  drawHudBottomLabel();

  fillRect(15, 300, 90, 5, 4);
  moveCursor(25,295);
  typeString(STAT_CATEGORY1, true);

  fillRect(115, 300, 100, 5, 4);
  moveCursor(125,295);
  typeString(STAT_CATEGORY2, true);

  fillRect(225, 300, 90, 5, 4);
  moveCursor(235,295);
  typeString(STAT_CATEGORY3, true);

  fillRect(325, 300, 65, 5, 4);
  moveCursor(335,295);
  typeString(STAT_CATEGORY4, true);

  fillRect(400, 300, 55, 5, 4);
  moveCursor(410,295);
  typeString(STAT_CATEGORY5, true);

  int16_t x = 0;
  int16_t y = 292;
  int16_t width = 1;
  int16_t height = 21;
  switch (selected) {
    default:
    case 0:
      x = 15;
      width = 90;
      break;
    case 1:
      x = 115;
      width = 100;
      break;
    case 2:
      x = 225;
      width = 90;
      break;
    case 3:
      x = 325;
      width = 65;
      break;
    case 4:
      x = 400;
      width = 55;
      break;
  }
  drawRect(x, y, width, height, 2);
}

uint16_t PipBoyDisplay::getCursorX() {
  return _tft.getCursorX();
}

uint16_t PipBoyDisplay::getCursorY() {
  return _tft.getCursorY();
}
