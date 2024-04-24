#include "logging.h"
#include "pins.h"
#include "lights.h"
#include "button.h"
#include "display.h"
#include "pipboot.h"

void setup() {
  Serial.begin(9600);
  debug("main", "booting...");
  initializeLights();
  initializeRotaryButton();
  initializeDisplay();
  bootPipBoy();
  debug("main", "finished, running loop...");
}

void loop() {
  updateCategoryLights();
  updateRotaryButton();
}
