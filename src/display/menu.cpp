#include "display/menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* menuItems[] = {
    "Envoyer un message",
    "Messages recus",
    "Parametres"
};

const int MENU_SIZE = 3;
int menuIndex = 0;

void afficherMenu(Adafruit_SSD1306 &display) {
    display.clearDisplay();

    // Encadré général
    display.drawRect(0, 0, 128, 64, WHITE);

    // Titre
    display.setCursor(40, 2);
    display.setTextColor(WHITE);
    display.print("Accueil");

    // Items
    for (int i = 0; i < MENU_SIZE; i++) {
        bool selected = (i == menuIndex);

        if (selected) {
            display.fillRect(2, 15 * i + 12, 124, 13, WHITE);
            display.setTextColor(BLACK);
        } else {
            display.setTextColor(WHITE);
        }

        display.setCursor(5, 15 * i + 15);
        display.print(menuItems[i]);
    }

    display.display();
}
