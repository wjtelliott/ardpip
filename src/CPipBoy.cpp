#include <Arduino.h>
#include "CPipBoy.h"
#include "pins.h"
#include "CWeapons.h"
#include "CStats.h"
#include "CConditions.h"
#include "CSpecials.h"
#include "CApparel.h"
#include "CAid.h"
#include "CHealth.h"
#include "CAmmo.h"
#include "CSomethingElses.h"
#include "CMiscs.h"
#include "CQuests.h"
#include "CNotes.h"
#include "CRadios.h"


#define NO_PAGE_ERROR "No Page!"

PipBoy::PipBoy() {}

//
// Used in calculating free memory.
//
extern unsigned int __bss_end;
extern void *__brkval;

//
// Returns the current amount of free memory in bytes.
//
int freeMemory() {
	int free_memory;
	if ((int) __brkval)
		return ((int) &free_memory) - ((int) __brkval);
	return ((int) &free_memory) - ((int) &__bss_end);
}

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

  // this looks gross for right now, but this is how it's gonna be until i make it more dry... :(
  // stats
  Item allStats[] = STATS;
  for (uint8_t i = 0; i < sizeof(allStats)/sizeof(allStats[0]) && i < 20; i++) {
    _pages[0]->pushItem(allStats[i]);
  }
  Item allConditions[] = CONDITIONS;
  for (uint8_t i = 0; i < sizeof(allConditions)/sizeof(allConditions[0]) && i < 20; i++) {
    _pages[0]->pushItem(allConditions[i]);
  }
  Item allSpecials[] = SPECIALS;
  for (uint8_t i = 0; i < sizeof(allSpecials)/sizeof(allSpecials[0]) && i < 20; i++) {
    _pages[0]->pushItem(allSpecials[i]);
  }
  Item allHealths[] = STATS_HEALTHS;
  for (uint8_t i = 0; i < sizeof(allHealths)/sizeof(allHealths[0]) && i < 20; i++) {
    _pages[0]->pushItem(allHealths[i]);
  }
  Item allSomethingElses[] = STATS_1;
  for (uint8_t i = 0; i < sizeof(allSomethingElses)/sizeof(allSomethingElses[0]) && i < 20; i++) {
    _pages[0]->pushItem(allSomethingElses[i]);
  }

  Serial.print("Free mem left: "); Serial.println(freeMemory());

  // inventory
  Item allPistols[] = PISTOLS;
  for (uint8_t i = 0; i < sizeof(allPistols)/sizeof(allPistols[0]) && i < 20; i++) {
    _pages[1]->pushItem(allPistols[i]);
  }
  Item allApparel[] = ARMORS;
  for (uint8_t i = 0; i < sizeof(allApparel)/sizeof(allApparel[0]) && i < 20; i++) {
    _pages[1]->pushItem(allApparel[i]);
  }
  Item allAids[] = MEDS;
  for (uint8_t i = 0; i < sizeof(allAids)/sizeof(allAids[0]) && i < 20; i++) {
    _pages[1]->pushItem(allAids[i]);
  }
  Item allAmmos[] = AMMOS;
  for (uint8_t i = 0; i < sizeof(allAmmos)/sizeof(allAmmos[0]) && i < 20; i++) {
    _pages[1]->pushItem(allAmmos[i]);
  }
  Item allMisc[] = ALL_MISCS;
  for (uint8_t i = 0; i < sizeof(allMisc)/sizeof(allMisc[0]) && i < 20; i++) {
    _pages[1]->pushItem(allMisc[i]);
  }
  Serial.print("Free mem left: "); Serial.println(freeMemory());

  // data page stuff
  // Item allWorldMap[] = PISTOLS;
  // for (uint8_t i = 0; i < sizeof(allWorldMap)/sizeof(allWorldMap[0]) && i < 20; i++) {
  //   _pages[2]->pushItem(allWorldMap[i]);
  // }
  // Item allLocalMap[] = ARMORS;
  // for (uint8_t i = 0; i < sizeof(allLocalMap)/sizeof(allLocalMap[0]) && i < 20; i++) {
  //   _pages[2]->pushItem(allLocalMap[i]);
  // }
  Item allQuests[] = QUESTS;
  for (uint8_t i = 0; i < sizeof(allQuests)/sizeof(allQuests[0]) && i < 20; i++) {
    _pages[2]->pushItem(allQuests[i]);
  }
  Item allNotes[] = NOTES;
  for (uint8_t i = 0; i < sizeof(allNotes)/sizeof(allNotes[0]) && i < 20; i++) {
    _pages[2]->pushItem(allNotes[i]);
  }
  Item allRadios[] = RADIOS;
  for (uint8_t i = 0; i < sizeof(allRadios)/sizeof(allRadios[0]) && i < 20; i++) {
    _pages[2]->pushItem(allRadios[i]);
  }
  Serial.print("Free mem left: "); Serial.println(freeMemory());
}

void PipBoy::updatePipLights() {
  char* pageName = _pages[_currentPage]->getPageName();
  digitalWrite(STAT_LIGHT, pageName == STAT_PAGE ? HIGH : LOW);
  digitalWrite(INV_LIGHT, pageName == INVENTORY_PAGE ? HIGH : LOW);
  digitalWrite(DATA_LIGHT, pageName == DATA_PAGE ? HIGH : LOW);
}

void PipBoy::boot() {
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
  auto x = _display.getCursorX();
  auto y = _display.getCursorY();
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

void PipBoy::assertPageInScope() {
  size_t pageCount = sizeof(_pages)/sizeof(_pages[0]);
  if (_currentPage < pageCount) return;
  Serial.println("ERR00: Page count out of bounds");
  for(;;);
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

char* PipBoy::getPageContents() {
  assertPageInScope();
  return _pages[_currentPage]->getContents();
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

void redrawPage(bool* needsRedraw, PipBoyDisplay* display, PipBoy* pip) {
  if (!*needsRedraw) return;
  display->clear();
  *needsRedraw = false;
  display->moveCursor(25,50);
  display->typeString(pip->getPageName(), true);
  display->typeString(" - ", true);
  display->typeStringLn(pip->getCategoryName(), true);
  display->typeStringLn(pip->getPageContents(), true);
}

void test(PipBoy* pip) {
  auto catName = pip->getCategoryName();
  pip->changeCategory(1);
  if (catName == STAT_CATEGORY5 || catName == INVENTORY_CATEGORY5 || catName == DATA_CATEGORY5) {
    pip->changePage(1);
  }
  delay(1750);
}

void PipBoy::tick() {
  // updatePipLights();
  // int8_t newPageDirection = getPageDirection();
  // if (newPageDirection) changePage(newPageDirection);
  // else {
  //   int8_t newCategoryDirection = getCategoryDirection();
  //   if (newCategoryDirection) changeCategory(newCategoryDirection);
  // }
  redrawPage(&_needsPageRedraw, &_display, this);
  test(this);
}

int* PipBoy::getLastPageSelectVoltage() {
  return &_lastPageSelectVoltage;
}

int* PipBoy::getLastCategoryVoltage() {
  return &_lastCategorySelectVoltage;
}

uint8_t getControllerTypeInfo(char* type, char* pinType) {
  if (type == "leftright") {
    return pinType == "CLK" ? PAGE_LEFT_RIGHT_CLK : PAGE_LEFT_RIGHT_DT;
  }
  if (type == "updown") {
    return pinType == "CLK" ? PAGE_UP_DOWN_CLK : PAGE_UP_DOWN_DT;
  }
  if (type == "page") {
    return pinType == "CLK" ? ENCODER_CLK : ENCODER_DT;
  }
  return 255;
}

int* getLastVoltagePointer(char* type, PipBoy* pip) {
  if (type == "leftright") return pip->getLastCategoryVoltage();
  // if (type == "updown") return &lastPageUpDownVoltage;
  if (type == "page") return pip->getLastPageSelectVoltage();
  return 255; // C doesn't throw, we just assume this won't break and hope for the best
}

int8_t getRotateStateOfRotaryButton(char* rotaryType, PipBoy* pip) {
  uint8_t CLK = getControllerTypeInfo(rotaryType, "CLK");
  uint8_t DT = getControllerTypeInfo(rotaryType, "DT");
  int *lastRotateVoltage = getLastVoltagePointer(rotaryType, pip);

  auto newRotateVoltage = digitalRead(CLK);
  if (*lastRotateVoltage == newRotateVoltage) return 0;
  *lastRotateVoltage = newRotateVoltage;
  if (newRotateVoltage == HIGH) return 0;

  auto direction = digitalRead(DT);
  delay(100);
  return direction == HIGH ? _CLOCKWISE : _COUNTER_CLOCKWISE;
}

// will return either a -1,0,1. this will apply to the change page function
int8_t PipBoy::getPageDirection() {
  return getRotateStateOfRotaryButton("page", this);
}

int8_t PipBoy::getCategoryDirection() {
  return getRotateStateOfRotaryButton("leftright", this);
}
