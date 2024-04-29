/***************************************************************************************
** File Name  :         CPipBoyPage.h
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         PipBoy page class. This class will handle the data to display on
**                      the current viewing page. It will also hold which categories
**                      are on the page.
***************************************************************************************/

#ifndef PipBoyPage_h
#define PipBoyPage_h

#include <Arduino.h>

class PipBoyPage {
  private:
    char* _name;
    uint8_t _currentPageCategory;
    uint8_t _currentHighlightedItem;
    char *_categories[5];
    uint16_t _items[24];
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
    void pushItem(uint16_t);
    void removeItem(uint16_t);

    uint16_t getItemCount();
    uint16_t getItemCode(uint16_t);
};

#endif