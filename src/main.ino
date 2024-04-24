#include "logging.h"
#include "pins.h"
#include "lights.h"
#include "button.h"

void setup() {
  Serial.begin(9600);
  debug("main", "booting...");
  initializeLights();
  initializeRotaryButton();
  debug("main", "finished, running loop...");
}

void loop() {
  updateCategoryLights();
  updateRotaryButton();
}
