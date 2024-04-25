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
      Serial.println("ERR01: Failed to initialize page");
      for(;;);
    }
    Serial.print("Add category: ");
    Serial.print(_categories[i]);
    Serial.print(" to page: ");
    Serial.println(_name);
  }
}

void PipBoyPage::assertCategoryInBounds() {
  size_t size = sizeof(_categories)/sizeof(_categories[0]);
  if (_currentPageCategory < size) return;
  Serial.println("ERR02: Page category out of bounds");
  for(;;);
}
void PipBoyPage::assertItemInBounds (){}

void PipBoyPage::moveHighlightedItem(int8_t direction) {}
void PipBoyPage::changePageCategory(int8_t direction) {}

char* PipBoyPage::getPageName() {
  return _name;
}
char* PipBoyPage::getCategoryName() {
  assertCategoryInBounds();
  return _categories[_currentPageCategory];
}
char* PipBoyPage::getHighlightedItem() {}

char* PipBoyPage::getAllCategoryNamesForPage() {}
char* PipBoyPage::getAllItemNamesForPage() {}