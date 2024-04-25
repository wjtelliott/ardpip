#define LIGHT_RESET 127
#define LIGHTS_SIZE sizeof(LIGHTS)/sizeof(LIGHTS[0])
const uint8_t LIGHTS[] = { STAT_LIGHT, INV_LIGHT, DATA_LIGHT };

char* getTabPageFromIndex(uint8_t idx) {
  // i hate this
  switch (idx) {
    case 0: return "Stat page";
    case 1: return "Inventory";
    case 2: return "Data page";
  }
}

// currentCategory will be based on the LIGHTS index
uint8_t currentCategory = 0;

void displayPageName() {
  char* outputPrefix = "current category is: ";
  char* currentPageName = getTabPageFromIndex(currentCategory);
  char buf[strlen(outputPrefix) + strlen(currentPageName)];
  strcpy(buf, outputPrefix);
  strcat(buf, currentPageName);
  debug("lights", buf);
}

void changeCategory(int8_t modifier) {
  if (modifier == LIGHT_RESET) {
    currentCategory = 0;
    displayPageName();
    return;
  }

  const uint8_t MIN_CATEGORY = 0;
  const uint8_t MAX_CATEGORY = LIGHTS_SIZE - 1;

  // modifier must be 0,1,-1
  int8_t sanitizedModifier = 0;
  if (modifier > 0) sanitizedModifier = 1;
  else if (modifier < 0) sanitizedModifier = -1;

  bool willAddCategory = sanitizedModifier > 0;
  bool willSubCategory = sanitizedModifier < 0;
  bool atMaxCategory = currentCategory == MAX_CATEGORY;
  bool atMinCategory = currentCategory == MIN_CATEGORY;

  // used to clamp overflow
  if (atMaxCategory && willAddCategory) currentCategory = MIN_CATEGORY;
  else if (atMinCategory && willSubCategory) currentCategory = MAX_CATEGORY;
  else currentCategory += sanitizedModifier;

  displayPageName();
}

void initializeLights() {
  debug("lights", "setting light pin modes");
  for (uint8_t light = 0; light < LIGHTS_SIZE; light++) {
    pinMode(LIGHTS[light], OUTPUT);
    digitalWrite(LIGHTS[light], LOW);
  }
}

void updateCategoryLights() {
  for (uint8_t light = 0; light < LIGHTS_SIZE; light++) {
    auto voltage = light == currentCategory ? HIGH : LOW;
    digitalWrite(LIGHTS[light], voltage);
  }
}