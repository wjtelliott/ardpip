#define _CLOCKWISE 1
#define _COUNTER_CLOCKWISE -1
#define INPUT_PINS_SIZE sizeof(inputPins)/sizeof(inputPins[0])
#define PULLUP_PINS_SIZE sizeof(pullupPins)/sizeof(pullupPins[0])

void initializeRotaryButton() {
  debug("rotary", "setting rotary pin modes");
  const uint8_t inputPins[] = {
    ENCODER_DT, ENCODER_CLK,
    PAGE_LEFT_RIGHT_CLK, PAGE_LEFT_RIGHT_DT,
    PAGE_UP_DOWN_CLK, PAGE_UP_DOWN_DT
  };
  const uint8_t pullupPins[] = { ENCODER_SW, PAGE_LEFT_RIGHT_SW, PAGE_UP_DOWN_SW };

  for (uint8_t pin = 0; pin < INPUT_PINS_SIZE; pin++) pinMode(inputPins[pin], INPUT);
  for (uint8_t pin = 0; pin < PULLUP_PINS_SIZE; pin++) pinMode(pullupPins[pin], INPUT_PULLUP);
}

bool wasClicked = false;
void updatePageSelectClick() {
  bool currentlyClicked = digitalRead(ENCODER_SW) == LOW;

  if (!currentlyClicked) {
    wasClicked = false;
    return;
  }
  if (currentlyClicked && wasClicked) return;

  wasClicked = true;
  debug("rotary", "button was clicked in");
  changeCategory(LIGHT_RESET);
}

auto lastPageSelectVoltage = HIGH;
auto lastPageUpDownVoltage = HIGH;
auto lastPageLeftRightVoltage = HIGH;
uint8_t getControllerTypeInfo(char* type, char* pinType) {
  if (type == "leftright") {
    return pinType == "CLK" ? PAGE_LEFT_RIGHT_CLK : PAGE_LEFT_RIGHT_DT;
  }
  if (type == "updown") {
    return pinType == "CLK" ? PAGE_UP_DOWN_CLK : PAGE_UP_DOWN_DT;
  }
  if (type == "page") {
    return pinType == "CLK" ? ENCODER_CLK : ENCODER_DT;
  }
  return 255;
}
int* getLastVoltagePointer(char* type) {
  if (type == "leftright") return &lastPageLeftRightVoltage;
  if (type == "updown") return &lastPageUpDownVoltage;
  if (type == "page") return &lastPageSelectVoltage;
  return 255; // C doesn't throw errors, so we are just going to assume this won't break and hope for the best
}
int8_t getStateOfPageController(char* pageControllerType) {
  uint8_t CLK = getControllerTypeInfo(pageControllerType, "CLK");
  uint8_t DT = getControllerTypeInfo(pageControllerType, "DT");
  int *lastRotateVoltage = getLastVoltagePointer(pageControllerType);

  auto newRotateVoltage = digitalRead(CLK);
  if (*lastRotateVoltage == newRotateVoltage) return 0;
  *lastRotateVoltage = newRotateVoltage;
  if (newRotateVoltage == HIGH) return 0;

  auto direction = digitalRead(DT);
  delay(100);
  return direction == HIGH ? _CLOCKWISE : _COUNTER_CLOCKWISE;
}

void updateRotaryButton() {
  updatePageSelectClick();
  int8_t newPageValue = getStateOfPageController("page");
  if (newPageValue) changeCategory(newPageValue);
}