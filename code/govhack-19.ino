// Govhack 19
//  CED 423
//  A display for public spaces
//  to show information about localities.
//  TODO:
/* User settings */
#include "settings.h"
#include "wifi.h"

/* Code begins */
#include <ESP8266WiFi.h>
#include <FS.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define FONT_SMALL "NotoSansBold15"
#define FONT_LARGE "NotoSansBold36"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define SLIDE_LENGTH REPEAT_TIME/SLIDE_NO // How long each slide should show (ms)
// #define

#define COLOUR_NO 13
#define COLOUR_LIGHT 4
int colours[] = {TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_BLUE, TFT_MAGENTA, TFT_LIGHTGREY, TFT_GREEN, TFT_CYAN, TFT_RED, TFT_YELLOW, TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK};

int lastUpdate = 0;
int counter = 0;

void setup() {
  Serial.begin(115200);
  // Initialise TFT first
  tft.init();
  tft.setRotation(3);
  randomSeed(analogRead(A0));

  if (!SPIFFS.begin()) {
    raiseError("Storage failed.");
  }

  bool font_missing = false;
  if (SPIFFS.exists("/NotoSansBold15.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/NotoSansBold36.vlw") == false) font_missing = true;

  if (font_missing) {
    raiseError("Fonts missing.");
  }

  // Show loading screen whilst device connects to WiFi.

  tft.loadFont(FONT_LARGE); // Large font for main data
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("Please wait...", SCREEN_W / 2, (SCREEN_H - 36) / 2, 20);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  /* TODO: Limit connection time, switch back to fallback */
  while (WiFi.status() != WL_CONNECTED && Serial.available() == 0) {
    yield(); // Wait until connected

  }
  tft.fillScreen(TFT_BLACK);
  bool wifiEnabled = (WiFi.status() == WL_CONNECTED);
}

void loop() {
  if (millis() - lastUpdate > REPEAT_TIME / SLIDE_NO || lastUpdate == 0) {
    int colour_selection = random(0, COLOUR_NO);
    updateText((colour_selection > COLOUR_LIGHT) ? TFT_BLACK : TFT_WHITE, colours[colour_selection], counter);
    counter++;
    if (counter >= SLIDE_NO) {
      counter = 0;
    }
    lastUpdate = millis();
  }
}

void updateText(int text_colour, int background_colour, int text) {
  tft.fillScreen(background_colour);
  tft.setTextColor(text_colour, background_colour);

  tft.loadFont(FONT_LARGE); // Large font for main data
  tft.drawCentreString(data[text], SCREEN_W / 2, (SCREEN_H - 36) / 2, 20);

  tft.loadFont(FONT_SMALL); // Small font for header and footer
  tft.drawCentreString(headers[text], SCREEN_W / 2, 5, 10);
  tft.drawCentreString(footers[text], SCREEN_W / 2, SCREEN_H - 20, 10);
}

void raiseError(String errorText) {
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_WHITE);
  tft.print("Internal error: ");
  tft.println(errorText);
  Serial.println(errorText); // In future, log to SPIFFS?
  while (1) yield(); // Stay here twiddling thumbs waiting
}
