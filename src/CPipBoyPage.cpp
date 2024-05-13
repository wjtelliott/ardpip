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


uint8_t getIndexOfCategory(PipBoyPage* page, char* categoryName) {
  size_t size = page->getCategoryAmount();
  for (uint8_t i = 0; i < size; i++) {
    if (categoryName == page->getCategoryNameAtIndex(i)) return i;
  }
  Serial.println("ERR03: Could not find index of category in page");
  for(;;);
}

uint8_t PipBoyPage::getPageCategory() {
  return _currentPageCategory;
}

void PipBoyPage::removeItem(uint16_t category, uint16_t itemIndex) {
  _items[category].deleteAt(itemIndex);
}

void PipBoyPage::pushItem(uint16_t category, Item* item) {
  _items[category].push_back(item);
}

uint8_t PipBoyPage::getCategoryAmount() {
  return sizeof(_categories)/sizeof(_categories[0]);
}

char* PipBoyPage::getAllCategoryNamesForPage() {}
LinkedList PipBoyPage::getAllItemNamesForPage() {
  assertCategoryInBounds();
  return _items[_currentPageCategory];
}