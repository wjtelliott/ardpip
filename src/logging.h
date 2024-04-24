#define IS_DEBUG true
#define TAG_SIZE sizeof(TAGS)/sizeof(TAGS[0])
const char* TAGS[] = {
  "rotary",
  "lights",
  "ili9341",
  "main"
};

void debug(char* tag, char* txt) {
  if (!IS_DEBUG) return;
  bool shouldDisplay = false;
  for (uint16_t idx = 0; idx < TAG_SIZE; idx++) {
    if (TAGS[idx] == tag) {
      shouldDisplay = true;
      break;
    }
  }
  if (!shouldDisplay) return;
  Serial.print("[");
  Serial.print(tag);
  Serial.print("] - ");
  Serial.println(txt);
}