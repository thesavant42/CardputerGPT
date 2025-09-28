#ifndef CONFIG_H
#define CONFIG_H

#include <pgmspace.h>

// We'll keep our literal strings in program memory to save RAM
// Adjust these to your own data
static const char ssid[]      PROGMEM = "YOUR_WIFI_SSID";
static const char password[]  PROGMEM = "YOUR_WIFI_PASSWORD";
static const char openaiKey[]     PROGMEM = "sk-12345";

// For any JSON handling
#define JSON_DOC_SIZE 4096

#endif
