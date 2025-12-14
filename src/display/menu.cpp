#include "display/menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" 

// Définition des éléments du menu
const char* menuItems[] = {
    "1. Envoyer un message",
    "2. Messages recus",
    "3. Parametres"
};

// Calcule la taille du menu
const int MENU_SIZE = sizeof(menuItems) / sizeof(menuItems[0]); 
// Index initial de sélection
int menuIndex = 0; 

/**
 * Affiche le menu principal sur l'écran OLED.
 * Le menuIndex est utilisé pour inverser les couleurs de l'élément sélectionné.
 */
void afficherMenu(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    for (int i = 0; i < MENU_SIZE; i++) {
        // Affiche chaque élément avec un décalage vertical
        display.setCursor(0, i * 15);
        
        if (i == menuIndex) {
            // Élément sélectionné : Texte noir sur fond blanc
            display.setTextColor(BLACK, WHITE); 
            display.print(">");
            display.print(menuItems[i]);
            // Réinitialise la couleur pour le reste
            display.setTextColor(WHITE);
        } else {
            // Élément non sélectionné : Texte blanc sur fond noir
            display.print(" ");
            display.print(menuItems[i]);
        }
    }
    display.display();
}

/**
 * Gère les entrées de l'encodeur (rotation et clic) pour la navigation.
 * Met à jour menuIndex et currentScreen.
 */
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen) {
    // 1. Gestion de la rotation de l'encodeur (Navigation)
    int delta = encoder_getDelta();
    
    if (delta != 0) {
        menuIndex += delta;

        // Limiter l'index de manière circulaire
        if (menuIndex < 0) {
            menuIndex = MENU_SIZE - 1;
        } else if (menuIndex >= MENU_SIZE) {
            menuIndex = 0;
        }
        // L'affichage est géré dans loop() après cette fonction, pas besoin de redraw ici.
    }

    // 2. Gestion du clic de l'encodeur (Sélection)
    ButtonEvent enc_event = encoder_getSwitchEvent();
    
    if (enc_event == BUTTON_SINGLE_CLICK) {
        // Validation : Change l'état de l'écran 
        // 0 (Menu) devient 1, 2, ou 3
        *currentScreen = menuIndex + 1; 
    }
    
    // Note : Le cas BUTTON_DOUBLE_CLICK et BUTTON_LONG_PRESS n'est pas utilisé dans le menu.
}