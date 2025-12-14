#include "display/menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" // Ajout de l'inclusion nécessaire pour utiliser l'encodeur

const char* menuItems[] = {
    "1. Envoyer un message",
    "2. Messages recus",
    "3. Parametres"
};

const int MENU_SIZE = sizeof(menuItems) / sizeof(menuItems[0]); 
int menuIndex = 0;

void afficherMenu(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    for (int i = 0; i < MENU_SIZE; i++) {
        display.setCursor(0, i * 15);
        
        if (i == menuIndex) {
            display.setTextColor(BLACK, WHITE); // Inverser les couleurs
            display.print(">");
            display.print(menuItems[i]);
            display.setTextColor(WHITE);
        } else {
            display.print(" ");
            display.print(menuItems[i]);
        }
    }
    display.display();
}

// DÉFINITION DE LA FONCTION MANQUANTE
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen) {
    // 1. Gestion de la rotation de l'encodeur (Navigation)
    int delta = encoder_getDelta();
    bool redraw = false;

    if (delta != 0) {
        menuIndex += delta;

        // Limiter l'index de manière circulaire
        if (menuIndex < 0) {
            menuIndex = MENU_SIZE - 1;
        } else if (menuIndex >= MENU_SIZE) {
            menuIndex = 0;
        }
        redraw = true;
    }

    // 2. Gestion du clic de l'encodeur (Sélection)
    ButtonEvent enc_event = encoder_getSwitchEvent();
    
    if (enc_event == BUTTON_SINGLE_CLICK) {
        // Validation : Change l'état de l'écran 
        // currentScreen (0) devient 1, 2, ou 3
        *currentScreen = menuIndex + 1; 
        redraw = true;
    }
    
    // 3. Affichage si une action a eu lieu
    if (redraw) {
        afficherMenu(display);
    }
}