#ifndef PipBoyPage_h
#define PipBoyPage_h

#include <Arduino.h>

struct Item {
  char* name;
  char* categoryName;
};

class PipBoyPage {
  private:
    char* _name;
    uint8_t _currentPageCategory;
    uint8_t _currentHighlightedItem;
    char *_categories[5];
    Item _items[7];
    void assertCategoryInBounds();
    void assertItemInBounds();
  public:
    PipBoyPage(char*);
    void setupPage();
    void moveHighlightedItem(int8_t);
    void changePageCategory(int8_t);

    char* getPageName();
    char* getCategoryName();
    char* getCategoryNameAtIndex(uint8_t);
    uint8_t getCategoryAmount();
    char* getHighlightedItem();

    char* getAllCategoryNamesForPage();
    char* getAllItemNamesForPage();
    char* getContents();
    void pushItem(Item);
};

#endif