#include <Arduino.h>
#include "CPipBoy.h"
#include "pins.h"
#define NO_PAGE_ERROR "No Page!"

PipBoy::PipBoy() {}

void PipBoy::init() {
  _currentPage = 0;
  _needsPageRedraw = true;
  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  for (uint8_t i = 0; i < pageCount; i++) {
    _pages[i]->setupPage();
  }

  _changePageRotary = { ENCODER_CLK, ENCODER_DT, ENCODER_SW };
  _pageHorizontalRotary = { PAGE_LEFT_RIGHT_CLK, PAGE_LEFT_RIGHT_DT, PAGE_LEFT_RIGHT_SW };
  _pageVerticalRotary = { PAGE_UP_DOWN_CLK, PAGE_UP_DOWN_DT, PAGE_UP_DOWN_SW };

  _display.init();
}

void PipBoy::boot() {
  char* garbage = malloc(0);
  buildGarbageBootData(garbage);
  printGarbageBootData(garbage, 5);
  free(garbage);
  _display.clear();
  _display.moveCursor(0, 0);
  printBootIntro();
  haltAndBlinkCursor(4);
  _display.clear();
}

void PipBoy::buildGarbageBootData(char* buffer) {
  free(buffer);
  const char* garbageIntro[] = {
    _s1, _s2, _s3, _str1,
    _s7, _s4, _s5, _str2,
    _s1, _s4, _s3, _str3,
    _s6, _s3, _s1, _str4,
    _s5, _s2, _s6, _str5,
    _s1, _s5, _s2, _str6
  };
  const uint8_t introSize = sizeof(garbageIntro)/sizeof(garbageIntro[0]);
  uint16_t stringLengthTotal = 0;
  for (uint8_t idx = 0; idx < introSize; idx++) {
    stringLengthTotal += strlen(garbageIntro[idx]);
  }

  buffer = (unsigned char *) malloc(stringLengthTotal);
  strcpy(buffer, "booting...");
  for (uint8_t idx = 0; idx < introSize; idx++) {
    strcat(buffer, garbageIntro[idx]);
  }
}

void PipBoy::printGarbageBootData(char* txt, uint8_t amount) {
  if (amount < 0) return;
  for (uint8_t i = 0; i < amount; i++) {
    _display.typeString(txt, true);
  }
}
void PipBoy::printBootIntro() {
  const char* bootIntro[] = {
    _boot1,
    _boot2,
    _boot3,
    _boot4,
    _boot5,
    _boot6,
    _boot7,
    _boot8,
    _boot9,
    _boot10,
    _boot11,
  };
  int size = sizeof(bootIntro)/sizeof(bootIntro[0]);
  for (uint8_t idx = 0; idx < size; idx++) {
    _display.typeStringLn(bootIntro[idx], false);
  }
}

void PipBoy::haltAndBlinkCursor(uint8_t amount) {
  if (amount < 1) return;
  auto x = _display.getCursorX();
  auto y = _display.getCursorY();
  auto width = 6;
  auto height = 10;
  auto blinkDelay = 500;
  
  for (uint8_t i = 0; i < amount; i++) {
    _display.fillRect(x, y, width, height, YELLOW);
    delay(blinkDelay);
    _display.fillRect(x, y, width, height, BLACK);
    delay(blinkDelay);
  }
}

void PipBoy::assertPageInScope() {
  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  if (_currentPage < pageCount) return;
  Serial.println("ERR00: Page count out of bounds");
  for(;;);
}

void PipBoy::moveHighlightedItem(int8_t direction) {}
void PipBoy::changePage(int8_t direction) {}
void PipBoy::changeCategory(int8_t direction) {}

char* PipBoy::getPageName() {
  assertPageInScope();
  return _pages[_currentPage]->getPageName();
}
char* PipBoy::getCategoryName() {
  assertPageInScope();
  return _pages[_currentPage]->getCategoryName();
}
char* PipBoy::getHighlightedItem() {
  assertPageInScope();
  return "highlight";
}

char* PipBoy::getAllPageNames() {
  return "";
}
char* PipBoy::getAllCategoryNamesForPage(uint8_t pageId) {
  return "";
}
char* PipBoy::getAllItemNamesForPage(uint8_t pageId) {
  return "";
}

void PipBoy::tick() {
  if (!_needsPageRedraw) return;
  _needsPageRedraw = true;
  // draw page name
  _display.moveCursor(0,0);
  _display.typeString(this->getPageName(), false);
  // draw page category
  _display.typeString(" - ", false);
  _display.typeString(this->getCategoryName(), false);
}
