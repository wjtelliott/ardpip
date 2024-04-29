/***************************************************************************************
** File Name  :         main.ino
** Author     :         WJTE
** Date       :         4/28/2024
** Description:         main entry of our program. `setup` will be called once on
**                      initialization. `loop` will be called afterwards indefinitely
***************************************************************************************/

// debug options (to be disabled before deploy)
//#define NO_BOOT
//#define NO_SERIAL_LOGS

#include "CPipBoy.h"

PipBoy pip;
void setup() {
  Serial.begin(9600);
  // Wait for serializer to init
  while(!Serial){};
  pip.init();
#ifndef NO_BOOT
  pip.boot();
#endif
}

void loop() {
  pip.tick();
}
