#define _s1 " 0x00000000000000000"
#define _s2 " 0x000000000000E003D"
#define _s3 " 0x0000A4"
#define _s4 " 0x000014"
#define _s5 " 0x000009"
#define _s6 " 1"
#define _s7 " 0"

#define _str1 " start memory discovery"
#define _str2 " CPU0 starting cell relocation"
#define _str3 " CPU0 launch EFIO"
#define _str4 " CPU0 starting EFIO"
#define _str5 " CPU0 loading user settings"
#define _str6 " CPU0 launch user"

#define _boot1 "*************** PIP-OS(R) V1.0 ***************"
#define _boot2 ""
#define _boot3 ""
#define _boot4 ""
#define _boot5 "COPYRIGHT 2075 ROBCO(R)"
#define _boot6 "LOADER V1.1"
#define _boot7 "EXEC VERSION 41.10"
#define _boot8 "64k RAM SYSTEM"
#define _boot9 "38911 BYTES FREE"
#define _boot10 "NO HOLOTAPE FOUND"
#define _boot11 "LOAD ROM(1): DEITRIX 303"
#define INTRO_SIZE sizeof(bootIntro)/sizeof(bootIntro[0])
const char* bootIntro[] = {
  _boot1,
  _boot2,
  _boot3,
  _boot4,
  _boot5,
  _boot6,
  _boot7,
  _boot8,
  _boot9,
  _boot10,
  _boot11,
};

void buildGarbageData(char* buffer) {
  free(buffer);
  const char* garbageIntro[] = {
    _s1, _s2, _s3, _str1,
    _s7, _s4, _s5, _str2,
    _s1, _s4, _s3, _str3,
    _s6, _s3, _s1, _str4,
    _s5, _s2, _s6, _str5,
    _s1, _s5, _s2, _str6
  };
  const uint8_t introSize = sizeof(garbageIntro)/sizeof(garbageIntro[0]);
  uint16_t stringLengthTotal = 0;
  for (uint8_t idx = 0; idx < introSize; idx++) {
    stringLengthTotal += strlen(garbageIntro[idx]);
  }

  buffer = (unsigned char *) malloc(stringLengthTotal);
  strcpy(buffer, "booting...");
  for (uint8_t idx = 0; idx < introSize; idx++) {
    strcat(buffer, garbageIntro[idx]);
  }
}

void printGarbageData(char* txt, uint8_t amount) {
  if (amount < 0) return;
  for (uint8_t i = 0; i < amount; i++) {
    typeStringOnDisplay(txt, true);
  }
}

void printBootIntro() {
  for (uint8_t idx = 0; idx < INTRO_SIZE; idx++) {
    typeStringLnOnDisplay(bootIntro[idx], false);
  }
}

void haltAndBlinkCursor(uint8_t amount) {
  if (amount < 1) return;
  auto x = getCursorX();
  auto y = getCursorY();
  auto width = 6;
  auto height = 10;
  auto blinkDelay = 500;
  
  for (uint8_t i = 0; i < amount; i++) {
    fillRectOnDisplay(x, y, width, height, YELLOW);
    delay(blinkDelay);
    fillRectOnDisplay(x, y, width, height, BLACK);
    delay(blinkDelay);
  }
}

void bootPipBoy() {
  char* garbageIntro = malloc(0);
  buildGarbageData(garbageIntro);
  printGarbageData(garbageIntro, 5);
  // buildGarbageData is not memory safe
  free(garbageIntro);
  clearDisplay();
  moveDisplayCursor(0, 0);

  printBootIntro();
  haltAndBlinkCursor(3);
}
