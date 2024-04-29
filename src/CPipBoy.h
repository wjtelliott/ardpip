/***************************************************************************************
** File Name  :         CPipBoy.h
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         Main refernce for tying our other classes and systems
**                      together to create our desired "pip boy" look and feel.
***************************************************************************************/

#ifndef PipBoy_h
#define PipBoy_h

#include <Arduino.h>
#include "CPipBoyPageNames.h"
#include "CPipBoyPage.h"
#include "CPipBoyDisplay.h"

// rotary direction macros
#define _CLOCKWISE 1
#define _COUNTER_CLOCKWISE -1

//---- Pipboy boot sequence//
#define _s1 " 0x00000000000000000"
#define _s2 " 0x000000000000E003D"
#define _s3 " 0x0000A4"
#define _s4 " 0x000014"
#define _s5 " 0x000009"
#define _s6 " 1"
#define _s7 " 0"

#define _str1 " start memory discovery"
#define _str2 " CPU0 starting cell relocation"
#define _str3 " CPU0 launch EFIO"
#define _str4 " CPU0 starting EFIO"
#define _str5 " CPU0 loading user settings"
#define _str6 " CPU0 launch user"

#define _boot1 "************ PIP-OS(R) V1.0 ***********"
#define _boot2 ""
#define _boot3 ""
#define _boot4 ""
#define _boot5 "COPYRIGHT 2075 ROBCO(R)"
#define _boot6 "LOADER V1.1"
#define _boot7 "EXEC VERSION 41.10"
#define _boot8 "64k RAM SYSTEM"
#define _boot9 "38911 BYTES FREE"
#define _boot10 "NO HOLOTAPE FOUND"
#define _boot11 "LOAD ROM(1): DEITRIX 303"
#define INTRO_SIZE sizeof(bootIntro)/sizeof(bootIntro[0])

struct Rotary {
  uint8_t CLK;
  uint8_t DT;
  uint8_t SW;
};

class PipBoy {
  private:
    uint8_t _currentPage;
    bool _needsPageRedraw;

    Rotary _changePageRotary = { PAGE_SELECT_CLK, PAGE_SELECT_DT, PAGE_SELECT_SW };
    Rotary _pageHorizontalRotary = { PAGE_LEFT_RIGHT_CLK, PAGE_LEFT_RIGHT_DT, PAGE_LEFT_RIGHT_SW };
    Rotary _pageVerticalRotary = { PAGE_UP_DOWN_CLK, PAGE_UP_DOWN_DT, PAGE_UP_DOWN_SW };

    const PipBoyPage *_pages[3] = {
      new PipBoyPage(STAT_PAGE),
      new PipBoyPage(INVENTORY_PAGE),
      new PipBoyPage(DATA_PAGE),
    };

    int _lastPageSelectVoltage = HIGH;
    int _lastCategorySelectVoltage = HIGH;

    const PipBoyDisplay _display;
    void buildGarbageBootData(char*);
    void printGarbageBootData(char*, uint8_t);
    void printBootIntro();
    void haltAndBlinkCursor(uint8_t);

    // exception checks
    void throwError(char*);
    void assertPageInScope();

    // init/loop funcs
    void updateLights();
    void updatePageSelectRotary();
    void updateHorizontalRotary();
    void updateVerticalRotary();

  public:
    PipBoy();
    // init/loop funcs
    void init();
    void boot();
    void tick();

    // ui management
    void updatePipLights();
    void moveHighlightedItem(int8_t);
    void changePage(int8_t);
    void changeCategory(int8_t);
    int8_t getPageDirection();
    int8_t getCategoryDirection();

    // getters
    char* getPageName();
    char* getCategoryName();
    char* getHighlightedItem();
    char* getAllPageNames();
    char* getAllCategoryNamesForPage(uint8_t);
    char* getAllItemNamesForPage(uint8_t);
    int* getLastPageSelectVoltage();
    int* getLastCategoryVoltage();
    uint8_t getControllerTypeInfo(char*, char*);
    uint16_t getPageItemCount();
    uint16_t getPageItemCode(uint16_t);
};

#endif