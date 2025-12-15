#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "menu.h"

extern Adafruit_SSD1306 display;

static const char* menuItems[] = {
    "Envoyer un message",
    "Messages recus",
    "Parametres"
};

static const int menuSize = 3;
static int selected = 0;

void menu_init() {
    selected = 0;
}

void menu_update(int delta, bool validate) {

    if (delta != 0)
        selected = (selected + delta + menuSize) % menuSize;

    display.clearDisplay();
    display.setTextSize(1);

    for (int i = 0; i < menuSize; i++) {
        display.setCursor(0, i * 10);
        if (i == selected)
            display.print("> ");
        else
            display.print("  ");
        display.println(menuItems[i]);
    }

    display.display();
}

int menu_getSelected() {
    return selected;
}
