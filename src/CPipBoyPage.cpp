#include <Arduino.h>
#include "CPipBoyPage.h"
#include "CPipBoyPageNames.h"

PipBoyPage::PipBoyPage(char* pageName) {
  _currentPageCategory = 0;
  _currentHighlightedItem = 0;
  _name = pageName;
  Item blankItem = {"", ""};
  _items[5];
  _items[0] = blankItem;
  _items[1] = blankItem;
  _items[2] = blankItem;
  _items[3] = blankItem;
  _items[4] = blankItem;
  _items[5] = blankItem;
  _items[6] = blankItem;
  _items[7] = blankItem;
  _items[8] = blankItem;
  _items[9] = blankItem;
  _items[10] = blankItem;
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

char* PipBoyPage::getContents() {
  char buffer[50];
  strcpy(buffer, "\n");
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    if (
      _items[i].categoryName != _categories[_currentPageCategory] ||
      _items[i].name == NULL
    ) {
      continue;
    }
    strcat(buffer, _items[i].name);
    strcat(buffer, "\n");
  }
  char* outputer = buffer;
  return outputer;
}

uint8_t getIndexOfCategory(PipBoyPage* page, char* categoryName) {
  size_t size = page->getCategoryAmount();
  for (uint8_t i = 0; i < size; i++) {
    if (categoryName == page->getCategoryNameAtIndex(i)) return i;
  }
  Serial.println("ERR03: Could not find index of category in page");
  for(;;);
}

void PipBoyPage::removeItem(Item item) {
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    if (_items[i].name == item.name && _items[i].categoryName == item.categoryName) {
      _items[i].name = "";
      _items[i].categoryName = "";
      return;
    }
  }
}

void PipBoyPage::pushItem(Item item) {
  for (uint8_t i = 0; i < sizeof(_items)/sizeof(_items[0]); i++) {
    if (_items[i].name == NULL || _items[i].name == "") {
      Serial.print("Pushing item: "); Serial.print(item.name); Serial.println(item.categoryName);
      _items[i] = item;
      return;
    }
  }
}

uint8_t PipBoyPage::getCategoryAmount() {
  return sizeof(_categories)/sizeof(_categories[0]);
}

char* PipBoyPage::getAllCategoryNamesForPage() {}
char* PipBoyPage::getAllItemNamesForPage() {}