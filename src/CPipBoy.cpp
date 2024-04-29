/***************************************************************************************
** File Name  :         CPipBoy.cpp
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         Implementation of our CPipBoy class
***************************************************************************************/

#include <Arduino.h>
#include "CPipBoy.h"
#include "pins.h"
#include "CItems.h"

// enable/disable this if we need to invert the horz rotary on the hardware
#define INVERT_LEFT_RIGHT_ROTARY

PipBoy::PipBoy() {}

/***************************************************************************************
** Helpers
***************************************************************************************/

void redrawPage(bool* needsRedraw, PipBoyDisplay* display, PipBoy* pip) {
  if (!*needsRedraw) return;
  display->clear();
  *needsRedraw = false;
  display->moveCursor(25,50);
  display->typeString(pip->getPageName(), true);
  display->typeString(" - ", true);
  display->typeStringLn(pip->getCategoryName(), true);
  display->typeStringLn("", true);
  uint16_t sizeOfItems = pip->getPageItemCount();
  for (uint16_t i = 0; i < sizeOfItems; i++) {
    uint16_t itemCode = pip->getPageItemCode(i);
    if (itemCode == 0) continue;
    BaseItem item = getItemData(itemCode);
    if (item.categoryName != pip->getCategoryName()) continue;
    display->typeStringLn(item.name, true);
  }
}

// use this to cycle through all pages when hardware isn't hooked up completely yet
void test(PipBoy* pip) {
  auto catName = pip->getCategoryName();
  pip->changeCategory(1);
  if (
    // change page when all categories are viewed
    catName == STAT_CATEGORY5 ||
    catName == INVENTORY_CATEGORY5 ||
    catName == DATA_CATEGORY5
  ) {
    pip->changePage(1);
  }
  delay(1750);
}

int* getLastVoltagePointer(char* type, PipBoy* pip) {
  if (type == "leftright") return pip->getLastCategoryVoltage();
  // if (type == "updown") return &lastPageUpDownVoltage;
  if (type == "page") return pip->getLastPageSelectVoltage();
}

int8_t getRotateStateOfRotaryButton(char* rotaryType, PipBoy* pip) {
  uint8_t CLK = pip->getControllerTypeInfo(rotaryType, "CLK");
  uint8_t DT = pip->getControllerTypeInfo(rotaryType, "DT");
  int *lastRotateVoltage = getLastVoltagePointer(rotaryType, pip);

  auto newRotateVoltage = digitalRead(CLK);
  if (*lastRotateVoltage == newRotateVoltage) return 0;
  *lastRotateVoltage = newRotateVoltage;
  if (newRotateVoltage == HIGH) return 0;

  auto direction = digitalRead(DT);
  delay(100);
  return direction == HIGH ? _CLOCKWISE : _COUNTER_CLOCKWISE;
}

/***************************************************************************************
** Void methods
***************************************************************************************/
void PipBoy::init() {
  _currentPage = 0;
  _needsPageRedraw = true;
  _display.init();

  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  for (uint8_t i = 0; i < pageCount; i++) {
    _pages[i]->setupPage();
  }

  Rotary rotaries[] = { _changePageRotary, _pageHorizontalRotary, _pageVerticalRotary };
  int outputPins[] = { STAT_LIGHT, INV_LIGHT, DATA_LIGHT };

  for (uint8_t i = 0; i < sizeof(rotaries)/sizeof(rotaries[0]); i++) {
    pinMode(rotaries[i].CLK, INPUT);
    pinMode(rotaries[i].DT, INPUT);
    pinMode(rotaries[i].SW, INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < sizeof(outputPins)/sizeof(outputPins[0]); i++) {
    pinMode(outputPins[i], OUTPUT);
  }


  // default inventory start
  const uint16_t statItemCodes[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
  const uint16_t invItemCodes[] = { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222 };
  const uint16_t dataItemCodes[] = { 800, 801, 802, 803, 804, 805, 806 };

  for (uint8_t i = 0; i < sizeof(statItemCodes)/sizeof(statItemCodes[0]); i++) _pages[0]->pushItem(statItemCodes[i]);
  for (uint8_t i = 0; i < sizeof(invItemCodes)/sizeof(invItemCodes[0]); i++) _pages[1]->pushItem(invItemCodes[i]);
  for (uint8_t i = 0; i < sizeof(dataItemCodes)/sizeof(dataItemCodes[0]); i++) _pages[2]->pushItem(dataItemCodes[i]);
}

void PipBoy::updatePipLights() {
  char* pageName = _pages[_currentPage]->getPageName();
  digitalWrite(STAT_LIGHT, pageName == STAT_PAGE ? HIGH : LOW);
  digitalWrite(INV_LIGHT, pageName == INVENTORY_PAGE ? HIGH : LOW);
  digitalWrite(DATA_LIGHT, pageName == DATA_PAGE ? HIGH : LOW);
}

void PipBoy::boot() {
  _display.clear();
  char* garbage = malloc(0);
  buildGarbageBootData(garbage);
  printGarbageBootData(garbage, 3);
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
  uint16_t x = _display.getCursorX();
  uint16_t y = _display.getCursorY();
  auto width = 12;
  auto height = 20;
  auto blinkDelay = 500;
  
  for (uint8_t i = 0; i < amount; i++) {
    _display.fillRect(x, y, width, height, YELLOW);
    delay(blinkDelay);
    _display.fillRect(x, y, width, height, BLACK);
    delay(blinkDelay);
  }
}

void PipBoy::throwError(char* message) {
  _display.displayError(message);
}

void PipBoy::assertPageInScope() {
  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  if (_currentPage < pageCount) return;
  throwError("Page index out of bounds.");
}

void PipBoy::moveHighlightedItem(int8_t direction) {}

void PipBoy::changePage(int8_t direction) {
  int8_t sanitizedDirection = 0;
  if (direction < 0) sanitizedDirection = -1;
  else if (direction > 0) sanitizedDirection = 1;
  if (_currentPage == 0 && sanitizedDirection == -1) {
    _currentPage = sizeof(_pages)/sizeof(_pages[0]) - 1;
  } else {
    _currentPage += sanitizedDirection;
    if (_currentPage >= sizeof(_pages)/sizeof(_pages[0])) _currentPage = 0;
  }
  if (sanitizedDirection) _needsPageRedraw = true;
}

void PipBoy::changeCategory(int8_t direction) {
  int8_t sanitizedDirection = 0;
  if (direction < 0) sanitizedDirection = -1;
  else if (direction > 0) sanitizedDirection = 1;
  _pages[_currentPage]->changePageCategory(sanitizedDirection);
  if (sanitizedDirection) _needsPageRedraw = true;
}

void PipBoy::tick() {
  updatePipLights();
  int8_t newPageDirection = getPageDirection();
  if (newPageDirection) changePage(newPageDirection);
  else {
    int8_t newCategoryDirection = getCategoryDirection();
    if (newCategoryDirection) changeCategory(newCategoryDirection);
  }
  redrawPage(&_needsPageRedraw, &_display, this);
  // test(this);
}

/***************************************************************************************
** Getters
***************************************************************************************/
uint8_t PipBoy::getControllerTypeInfo(char* type, char* pinType) {
  if (type == "leftright") {
    return pinType == "CLK" ? _pageHorizontalRotary.CLK : _pageHorizontalRotary.DT;
  }
  if (type == "updown") {
    return pinType == "CLK" ? _pageVerticalRotary.CLK : _pageVerticalRotary.DT;
  }
  if (type == "page") {
    return pinType == "CLK" ? _changePageRotary.CLK : _changePageRotary.DT;
  }
  return 255;
}

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

uint16_t PipBoy::getPageItemCount() {
  return _pages[_currentPage]->getItemCount();
}

uint16_t PipBoy::getPageItemCode(uint16_t idx) {
  return _pages[_currentPage]->getItemCode(idx);
}

int* PipBoy::getLastPageSelectVoltage() {
  return &_lastPageSelectVoltage;
}

int* PipBoy::getLastCategoryVoltage() {
  return &_lastCategorySelectVoltage;
}

// will return either a -1,0,1. this will apply to the change page function
int8_t PipBoy::getPageDirection() {
  return getRotateStateOfRotaryButton("page", this);
}

// for some reason this is being inverted on our hardware???
int8_t PipBoy::getCategoryDirection() {
  int8_t rotaryDirection = getRotateStateOfRotaryButton("leftright", this);
#ifdef INVERT_LEFT_RIGHT_ROTARY
  return -rotaryDirection;
#endif
  return rotaryDirection;
}
