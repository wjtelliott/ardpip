/***************************************************************************************
** File Name  :         CPipBoyDisplay.h
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         Interface from our tft library to our codebase. Handles changing
**                      the text color, font, size, and displaying text and images on the
**                      screen.
***************************************************************************************/

#ifndef PipBoyDisplay_h
#define PipBoyDisplay_h

#include <TFT_HX8357.h>
#include "Free_Fonts.h"

#include "pins.h"

#define HORIZONTAL 1
#define DEFAULT_TEXT_SIZE 2
#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define BLACK 4
#define WRITE_DELAY 30
#define FAST_WRITE_DELAY 0

class PipBoyDisplay {
  private:
    TFT_HX8357 _tft;
    const uint16_t _COLORS[5] = {
      TFT_RED,
      TFT_BLUE,
      TFT_GREEN,
      TFT_YELLOW,
      TFT_BLACK
    };
    const size_t _COLORS_SIZE = sizeof(_COLORS)/sizeof(_COLORS[0]);
  public:
    PipBoyDisplay();
    void init();
    void changeTextSize(uint8_t);
    void changeTextColor(uint8_t);
    void moveCursor(uint16_t, uint16_t);
    void typeString(char*, bool);
    void typeStringLn(char*, bool);
    void clear();
    void drawRect(int16_t, int16_t, int16_t, int16_t, uint16_t);
    void drawLine(int16_t, int16_t, int16_t, int16_t, uint16_t);
    void fillRect(int16_t, int16_t, int16_t, int16_t, uint16_t);
    void displayError(char*);
    void drawSDImage(char*);
    void drawImage(uint16_t, uint16_t, uint8_t*, uint16_t, uint16_t, uint16_t);

    void drawHudTopLabel();
    void clearTopHud();
    void drawHudBottomLabel();
    void clearBottomHud();
    void drawItemPageTopHud();
    void drawStatPageTopHud();
    void drawTopHud(uint8_t);
    void drawBottomHud(uint8_t, uint8_t);
    void drawItemPageBottomHud(uint8_t);
    void drawStatPageBottomHud(uint8_t);

    uint16_t getCursorX();
    uint16_t getCursorY();
};

#endif
