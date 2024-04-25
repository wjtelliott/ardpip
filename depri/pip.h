#define STAT_PAGE 0
#define INV_PAGE 1
#define DATA_PAGE 2

struct itemHighlight {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};

uint8_t getCurrentPage() {
  // currently this is controlled by the lights.h
  // what a mess
  return currentCategory;
}

itemHighlight selectedItem = {0, 0, 50, 15};
void updateUpDownOnPage(int direction) {
  auto directionAmount = 15 * direction;
  drawRectOnDisplay(selectedItem.x, selectedItem.y, selectedItem.width, selectedItem.height, BLACK);
  selectedItem.y += directionAmount;
  drawRectOnDisplay(selectedItem.x, selectedItem.y, selectedItem.width, selectedItem.height, YELLOW);
}

//todo - fix
char* getPageCategoryName(uint8_t page, uint8_t *category, int direction) {
  switch (page) {
    case STAT_PAGE:
      switch (*category) {
        case 0: return "STATS";
        case 1: return "CONDITION";
        case 2: return "S.P.E.C.I.A.L.";
        case 3: return "HEALTH";
        case 4: return "SOMETHING ELSE";
        default:
          *category = direction > 0 ? 0 : 4;
          return getPageCategoryName(page, category, direction);
      }
    case INV_PAGE:
      switch (*category) {
        case 0: return "WEAPONS";
        case 1: return "APPAREAL";
        case 2: return "AID";
        case 3: return "AMMO";
        case 4: return "MISC";
        default:
          *category = direction > 0 ? 0 : 4;
          return getPageCategoryName(page, category, direction);
      }
    case DATA_PAGE:
      switch (*category) {
        case 0: return "WORLD MAP";
        case 1: return "LOCAL MAP";
        case 2: return "QUESTS";
        case 3: return "NOTES";
        case 4: return "RADIO";
        default:
          *category = direction > 0 ? 0 : 4;
          return getPageCategoryName(page, category, direction);
      }
      break;
    default:
      bootPipBoy(); // we done fucked up
      break;
  }
}

int8_t pageCategory = 0;
int lastDirection = 0;
void drawCategoryName() {
  moveDisplayCursor(75, 0);
  char* displayChar = getPageCategoryName(getCurrentPage(), &pageCategory, lastDirection);
  typeStringLnOnDisplay(displayChar, true);
}

void drawNewPage(uint8_t page) {
  clearDisplay();
  moveDisplayCursor(0, 0);
  char* pageName = getTabPageFromIndex(page);
  switch (page) {
    case STAT_PAGE:
    case INV_PAGE:
    case DATA_PAGE:
      typeStringLnOnDisplay(pageName, false);
      selectedItem = {0, 0, 50, 15};
      updateUpDownOnPage(0);
      if (!lastDirection) pageCategory = 0;
      drawCategoryName();
      break;
    default:
      bootPipBoy(); // we done fucked up
      break;
  }
}

void updateLeftRightOnPage(int direction) {
  pageCategory += direction;
  lastDirection = direction;
  drawNewPage(getCurrentPage());
}

void drawPageUpdate() {
  int upDown = getStateOfPageController("updown");
  if (upDown) {
    updateUpDownOnPage(upDown);
    return;
  }

  int leftRight = getStateOfPageController("leftright");
  if (leftRight) updateLeftRightOnPage(leftRight);
}

uint8_t lastCategoryDrawn = 255;
void updatePipBoy() {
  uint8_t state = getCurrentPage();
  if (state == lastCategoryDrawn) {
    drawPageUpdate();
    return;
  }
  lastCategoryDrawn = state;
  lastDirection = 0;
  drawNewPage(state);
}