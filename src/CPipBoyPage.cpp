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
void PipBoyPage::changePageCategory(int8_t direction) {
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

char* PipBoyPage::getContents() {
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    Serial.println(i);
  }
  return "none";
}

uint8_t getIndexOfCategory(PipBoyPage* page, char* categoryName) {
  size_t size = page->getCategoryAmount();
  for (uint8_t i = 0; i < size; i++) {
    if (categoryName == page->getCategoryNameAtIndex(i)) return i;
  }
  Serial.println("ERR03: Could not find index of category in page");
  for(;;);
}

void PipBoyPage::pushItem(Item item) {
  auto categoryIndex = getIndexOfCategory(this, item.categoryName);
  Item tmp[sizeof(_items)];
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    tmp[i] = _items[i];
  }
  free(_items);
  _items = (Item[]) malloc(sizeof(tmp)/sizeof(tmp[0]) + sizeof(tmp[0]));
  for (uint8_t i = 0; i < sizeof(tmp)/sizeof(tmp[0]); i++) {
    _items[i] = tmp[i];
  }
  _items[sizeof(_items)/sizeof(_items[0])] = item;
}

uint8_t PipBoyPage::getCategoryAmount() {
  return sizeof(_categories)/sizeof(_categories[0]);
}

char* PipBoyPage::getAllCategoryNamesForPage() {}
char* PipBoyPage::getAllItemNamesForPage() {}