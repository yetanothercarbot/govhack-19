// Govhack 19
//  A display for public spaces
//  to show information about localities.

/* User settings */
#include "settings.h"

/* Code begins */
#include <FS.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define FONT_SMALL "NotoSansBold15"
#define FONT_LARGE "NotoSansBold36"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define SLIDE_LENGTH REPEAT_TIME/SLIDE_NO // How long each slide should show (ms)

#define COLOUR_NO 13
#define COLOUR_LIGHT 4
int colours[] = {TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_BLUE, TFT_MAGENTA, TFT_LIGHTGREY, TFT_GREEN, TFT_CYAN, TFT_RED, TFT_YELLOW, TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK};

int lastUpdate = 0;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(3);
  randomSeed(analogRead(A0));

  if (!SPIFFS.begin()) {
    raiseError();
  }

  bool font_missing = false;
  if (SPIFFS.exists("/NotoSansBold15.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/NotoSansBold36.vlw") == false) font_missing = true;

  if (font_missing) {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1) yield();
  }
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

  tft.loadFont(FONT_SMALL); // Small font for header and footer
  tft.drawCentreString(headers[text], SCREEN_W / 2, 5, 8);
  tft.drawCentreString(footers[text], SCREEN_W / 2, SCREEN_H - 20, 8);

  tft.loadFont(FONT_LARGE); // Small font for header and footer
  tft.drawCentreString(data[text], SCREEN_W / 2, (SCREEN_H -36)/ 2, 20);
}

void raiseError() {
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  tft.println("Internal error!");
  while (1) yield(); // Stay here twiddling thumbs waiting
}
