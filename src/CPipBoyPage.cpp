/***************************************************************************************
** File Name  :         CPipBoyPage.cpp
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         Implementation of the PipBoyPage class
***************************************************************************************/

#include <Arduino.h>
#include "CPipBoyPage.h"
#include "CPipBoyPageNames.h"

PipBoyPage::PipBoyPage(char* pageName) {
  _currentPageCategory = 0;
  _currentHighlightedItem = 0;
  _name = pageName;
  _items[0] = 0;
  _items[1] = 0;
  _items[2] = 0;
  _items[3] = 0;
  _items[4] = 0;
  _items[5] = 0;
  _items[6] = 0;
  _items[7] = 0;
  _items[8] = 0;
  _items[9] = 0;
  _items[10] = 0;
  _items[11] = 0;
  _items[12] = 0;
  _items[13] = 0;
  _items[14] = 0;
  _items[15] = 0;
  _items[16] = 0;
  _items[17] = 0;
  _items[18] = 0;
  _items[19] = 0;
  _items[20] = 0;
  _items[21] = 0;
  _items[22] = 0;
  _items[23] = 0;
};

void PipBoyPage::setupPage() {
  size_t size = sizeof(_categories)/sizeof(_categories[0]);
  char* stats[] = STAT_CATEGORIES;
  char* inv[] = INVENTORY_CATEGORIES;
  char* data[] = DATA_CATEGORIES;
  for (uint8_t i = 0; i < size; i++) {
    if (_name == STAT_PAGE) {
      _categories[i] = stats[i];
    } else if (_name == INVENTORY_PAGE) {
      _categories[i] = inv[i];
    } else if (_name == DATA_PAGE) {
      _categories[i] = data[i];
    } else {
#ifndef NO_SERIAL_LOGS
      Serial.println("ERR01: Failed to initialize page");
#endif
      for(;;);
    }
  }
}

void PipBoyPage::assertCategoryInBounds() {
  size_t size = sizeof(_categories)/sizeof(_categories[0]);
  if (_currentPageCategory < size) return;
#ifndef NO_SERIAL_LOGS
  Serial.println("ERR02: Page category out of bounds");
#endif
  for(;;);
}
void PipBoyPage::assertItemInBounds (){}

void PipBoyPage::moveHighlightedItem(int8_t direction) {}
void PipBoyPage::changePageCategory(int8_t direction) {
  if (_currentPageCategory == 0 && direction < 0) {
    _currentPageCategory = sizeof(_categories)/sizeof(_categories[0]) - 1;
    return;
  }
  _currentPageCategory += direction;
  if (_currentPageCategory >= sizeof(_categories)/sizeof(_categories[0])) {
    _currentPageCategory = 0;
  }
}

char* PipBoyPage::getPageName() {
  return _name;
}
char* PipBoyPage::getCategoryName() {
  assertCategoryInBounds();
  return _categories[_currentPageCategory];
}
char* PipBoyPage::getCategoryNameAtIndex(uint8_t idx) {
  return _categories[idx];
}
char* PipBoyPage::getHighlightedItem() {}

uint16_t PipBoyPage::getItemCode(uint16_t idx) {
  return _items[idx];
}

uint16_t PipBoyPage::getItemCount() {
  return sizeof(_items)/sizeof(_items[0]);
}

uint8_t getIndexOfCategory(PipBoyPage* page, char* categoryName) {
  size_t size = page->getCategoryAmount();
  for (uint8_t i = 0; i < size; i++) {
    if (categoryName == page->getCategoryNameAtIndex(i)) return i;
  }
  Serial.println("ERR03: Could not find index of category in page");
  for(;;);
}

void PipBoyPage::removeItem(uint16_t itemCode) {
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    if (_items[i] == itemCode) {
      _items[i] = 0;
      return;
    }
  }
}

void PipBoyPage::pushItem(uint16_t itemCode) {
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    if (_items[i] == 0) {
      _items[i] = itemCode;
      return;
    }
  }
}

uint8_t PipBoyPage::getCategoryAmount() {
  return sizeof(_categories)/sizeof(_categories[0]);
}

char* PipBoyPage::getAllCategoryNamesForPage() {}
char* PipBoyPage::getAllItemNamesForPage() {}