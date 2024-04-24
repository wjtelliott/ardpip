#define _CLOCKWISE 1
#define _COUNTER_CLOCKWISE -1

void initializeRotaryButton() {
  debug("rotary", "setting rotary pin modes");
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);
}

bool wasClicked = false;
void updateClick() {
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

auto lastRotateV = HIGH;
void updateRotate() {
  auto newRotateV = digitalRead(ENCODER_CLK);
  if (lastRotateV == newRotateV) return;
  lastRotateV = newRotateV;

  auto direction = digitalRead(ENCODER_DT);
  if (newRotateV == HIGH) return;

  debug("rotary", direction == HIGH ? "rotate clockwise" : "rotate counter-clockwise");
  changeCategory(direction == HIGH ? _CLOCKWISE : _COUNTER_CLOCKWISE);
}

void updateRotaryButton() {
  updateClick();
  updateRotate();
}