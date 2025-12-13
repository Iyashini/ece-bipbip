#include "display/animation.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Bitmap logo ECE (128x64)
const unsigned char logoECE[] PROGMEM = {
    // <-- colle ici TOUT ton tableau bitmap complet -->
};

void animationDemarrage(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.display();

    // ---- FADE-IN ----
    for (int level = 0; level < 256; level += 16) {
        display.clearDisplay();
        display.drawBitmap(0, 0, logoECE, 128, 64, 1);
        display.dim(level < 128);
        display.display();
        delay(50);
    }

    delay(300);

    // ---- MACHINE À ÉCRIRE ----
    const char* text = "    BIP-BIP ECE";
    String temp = "";
    size_t len = strlen(text);

    for (size_t i = 0; i < len; i++) {
        temp += text[i];

        display.clearDisplay();
        display.drawBitmap(0, 0, logoECE, 128, 64, 1);

        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(10, 52);
        display.print(temp);

        display.display();
        delay(35);
    }

    delay(200);

    // ---- FADE-OUT ----
    for (int level = 255; level >= 0; level -= 16) {
        display.clearDisplay();
        display.drawBitmap(0, 0, logoECE, 128, 64, 1);
        display.dim(level < 128);
        display.display();
        delay(30);
    }

    display.clearDisplay();
    display.display();
}
