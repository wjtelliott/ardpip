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
    void assertCategoryInBounds();
    void assertItemInBounds();
  public:
    PipBoyPage(char*);
    void setupPage();
    void moveHighlightedItem(int8_t);
    void changePageCategory(int8_t);

    char* getPageName();
    char* getCategoryName();
    char* getHighlightedItem();

    char* getAllCategoryNamesForPage();
    char* getAllItemNamesForPage();
};

#endif