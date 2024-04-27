#include "CPipBoy.h"

PipBoy pip;
void setup() {
  Serial.begin(9600);
  while(!Serial){};
  pip.init();
  pip.boot();
}

void loop() {
  pip.tick();
}
