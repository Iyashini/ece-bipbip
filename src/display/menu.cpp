#include "display/menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" 

const char* menuItems[] = {
    "Envoyer un message",
    "Parametres",
    "......"
};

const int MENU_SIZE = sizeof(menuItems) / sizeof(menuItems[0]); 
int menuIndex = 0; // CORRECTION : Index initial à 0

void afficherMenu(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    for (int i = 0; i < MENU_SIZE; i++) {
        display.setCursor(0, i * 15);
        
        if (i == menuIndex) {
            display.setTextColor(BLACK, WHITE); 
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

void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen) {
    int delta = encoder_getDelta();
    ButtonEvent enc_event = encoder_getSwitchEvent();
    
    if (delta != 0) {
        menuIndex += delta;

        if (menuIndex < 0) {
            menuIndex = MENU_SIZE - 1;
        } else if (menuIndex >= MENU_SIZE) {
            menuIndex = 0;
        }
    }

    if (enc_event == BUTTON_SINGLE_CLICK) {
        *currentScreen = menuIndex + 1; 
    }
}