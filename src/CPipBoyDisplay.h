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
#define FAST_WRITE_DELAY 3

class PipBoyDisplay {
  private:
    TFT_HX8357 _tft;       // Invoke custom library
    // Adafruit_ILI9341 _tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    uint8_t _textSize;
    uint16_t _textColor;
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
    void drawRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
    void fillRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
    uint8_t getCursorX();
    uint8_t getCursorY();
};

#endif
