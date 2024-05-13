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
#include "CLinkedList.h"

// enable/disable this if we need to invert the horz rotary on the hardware
#define INVERT_LEFT_RIGHT_ROTARY

PipBoy::PipBoy() {}

/***************************************************************************************
** Helpers
***************************************************************************************/

//todo: this could probably be worked into the PipBoy tick func somehow or something
void redrawPage(bool* needsRedraw, PipBoyDisplay* display, PipBoy* pip) {
  if (!*needsRedraw) return;
  *needsRedraw = false;

  if (pip->isRedrawingNewPage()) {
    display->clearTopHud();
    display->drawTopHud(pip->getCurrentPage()); // todo, replace with "drawPageTop" when impl
    pip->setIsRedrawingNewPage(false);
  }
  display->clearBottomHud();
  display->drawBottomHud(pip->getCurrentPage(), pip->getPageCategory());

  display->fillRect(0, 80, 460, 200, 4);

  LinkedList items = pip->getAllItemNamesForPage();
  Item *curr = items.loop();
  display->moveCursor(0, 80);
  while (curr != nullptr) {
    display->typeString("   ", true);
    display->typeString(curr->name, true);
    char buf[8];
    strcpy(buf, " (");
    char quantityAsStr[3];
    itoa(curr->quantity, quantityAsStr, 10);
    strcat(buf, quantityAsStr);
    strcat(buf, ")");
    display->typeStringLn(curr->quantity > 1 ? buf : "", true);
    curr = items.nextItem();
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

// these funcs could prob be in their own file
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

//todo: move this to a new file + populate with correct + more data
void setUpStatPage(PipBoyPage *page) {
  // status
  page->pushItem(0, getItem(STAT_HEALTH_ID));
  page->pushItem(0, getItem(STAT_RADIATION_ID));
  page->pushItem(0, getItem(STAT_LEVEL_ID));

  // s.p.e.c.i.a.l.s
  page->pushItem(1, getItem(SPECIAL_STRENGTH_ID));
  page->pushItem(1, getItem(SPECIAL_PERCEPTION_ID));
  page->pushItem(1, getItem(SPECIAL_ENDURANCE_ID));
  page->pushItem(1, getItem(SPECIAL_CHARISMA_ID));
  page->pushItem(1, getItem(SPECIAL_INTELLIGENCE_ID));
  page->pushItem(1, getItem(SPECIAL_AGILITY_ID));
  page->pushItem(1, getItem(SPECIAL_LUCK_ID));

  // skills
  page->pushItem(2, getItem(SKILLS_BARTER_ID));
  page->pushItem(2, getItem(SKILLS_BIG_GUNS_ID));

  // perks
  page->pushItem(3, getItem(PERKS_ANT_SIGHT_ID));
  page->pushItem(3, getItem(PERKS_COMMANDO_ID));

  // general
  page->pushItem(4, getItem(GENERAL_QUESTS_COMPLETED_ID));
  page->pushItem(4, getItem(GENERAL_LOCATIONS_DISCOVERED_ID));
}

/***************************************************************************************
** Void methods
***************************************************************************************/
void PipBoy::init() {
  // todo: we can probably free or delete some memory after we've used it to init. like `rotaries[]`

  _currentPage = 0;
  _needsPageRedraw = true;
  _isRedrawingNewPage = true;
  _display.init();

  delay(2000);

  for (uint8_t i = 0; i < PAGE_SIZE; i++) {
    _pages[i]->setupPage();
  }

  Rotary rotaries[] = { _changePageRotary, _pageHorizontalRotary, _pageVerticalRotary };
  int outputPins[] = { STAT_LIGHT, INV_LIGHT, DATA_LIGHT };

  for (uint8_t i = 0; i < ROTARY_COUNT; i++) {
    pinMode(rotaries[i].CLK, INPUT);
    pinMode(rotaries[i].DT, INPUT);
    pinMode(rotaries[i].SW, INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < OUTPUT_PIN_COUNT; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  // set up default inventory
  setUpStatPage(_pages[0]);
  //todo when they exist:
  // setUpInvPage(_pages[1]);
  // setUpDataPage(_pages[2]);
}

void PipBoy::updatePipLights() {
  char* pageName = _pages[_currentPage]->getPageName();
  digitalWrite(STAT_LIGHT, pageName == STAT_PAGE ? HIGH : LOW);
  digitalWrite(INV_LIGHT, pageName == INVENTORY_PAGE ? HIGH : LOW);
  digitalWrite(DATA_LIGHT, pageName == DATA_PAGE ? HIGH : LOW);
}

void PipBoy::boot() {
  _display.clear();
  //todo: buildGarbageBootData isn't memory safe and we must free later. make it safe
  char* garbage = malloc(0);
  buildGarbageBootData(garbage);
  printGarbageBootData(garbage, 3);
  free(garbage);
  _display.clear();
  _display.moveCursor(0, 0);
  printBootIntro();
  haltAndBlinkCursor(4);
  _display.clear();
  _display.drawSDImage("fallout.bmp");
  delay(1500);
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
    _display.fillRect(x, y, width, height, GREEN);
    delay(blinkDelay);
    _display.fillRect(x, y, width, height, BLACK);
    delay(blinkDelay);
  }
}

//todo: error message should prob be more fleshed out later
void PipBoy::throwError(char* message) {
  _display.displayError(message);
}

void PipBoy::assertPageInScope() {
  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  if (_currentPage < pageCount) return;
  throwError("Page index out of bounds.");
}

// todo: draw a box around the currently highlighted item in the page
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
  if (sanitizedDirection) {
    _isRedrawingNewPage = true;
    _needsPageRedraw = true;
  }
}

void PipBoy::changeCategory(int8_t direction) {
  int8_t sanitizedDirection = 0;
  if (direction < 0) sanitizedDirection = -1;
  else if (direction > 0) sanitizedDirection = 1;
  _pages[_currentPage]->changePageCategory(sanitizedDirection);
  if (sanitizedDirection) _needsPageRedraw = true;
}

// todo: delete before deploy. this is just here to sandbox things inside the scope of the classes
void PipBoy::sandbox() {
  for (uint8_t i = 0; i <= 4; i++) {
    _display.drawItemPageTopHud();
    _display.clearBottomHud();
    _display.drawItemPageBottomHud(i);
    delay(1250);
  }
  Serial.println("done");
  for(;;);
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
bool PipBoy::isRedrawingNewPage() {
  return _isRedrawingNewPage;
}

uint8_t PipBoy::getPageCategory() {
  return _pages[_currentPage]->getPageCategory();
}

uint8_t PipBoy::getCurrentPage() {
  return _currentPage;
}

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
LinkedList PipBoy::getAllItemNamesForPage() {
  assertPageInScope();
  return _pages[_currentPage]->getAllItemNamesForPage();
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

// setters
void PipBoy::setIsRedrawingNewPage(bool val) {
  _isRedrawingNewPage = val;
}
