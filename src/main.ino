#include "CPipBoy.h"

PipBoy pip;
void setup() {
  Serial.begin(9600);
  delay(50);
  pip.init();
  pip.boot();
}

void loop() {
  pip.tick();
}
