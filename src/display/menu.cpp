#include "display/menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" 
#include <string.h> // Nécessaire pour strlen, si ce n'est pas déjà inclus.

const char* menuItems[] = {
    "Envoyer un message",
    "Messages recus",
    "Parametres"
};

const int MENU_SIZE = sizeof(menuItems) / sizeof(menuItems[0]); 
// Index initial de sélection
int menuIndex = 0; 

/**
 * Affiche le menu principal sur l'écran OLED avec titre et encadré.
 */
void afficherMenu(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // 1. Titre "ACCUEIL"
    const char* title = "ACCUEIL";
    int title_width = strlen(title) * 6; // Calcul de la largeur pour setTextSize(1)
    int x_center = (128 - title_width) / 2;
    
    display.setCursor(x_center, 4);
    display.print(title);
    
    // 2. Séparateur sous le titre
    display.drawLine(0, 12, 128, 12, WHITE);

    // 3. Affichage des items de menu
    for (int i = 0; i < MENU_SIZE; i++) {
        // Position verticale ajustée (commence à Y=18 pour laisser de la place au titre)
        // Intervalle de 14px entre les lignes
        int y_pos = 18 + i * 14; 
        
        if (i == menuIndex) {
            // Élément sélectionné : Texte noir sur fond blanc, sur toute la largeur
            // Le rectangle fait 128px de large et 12px de haut (y_pos - 2 pour un peu de marge)
            display.fillRect(0, y_pos - 2, 128, 12, WHITE);
            display.setTextColor(BLACK); 
            display.setCursor(5, y_pos);
            display.print(">");
            display.print(menuItems[i]);
            
            display.setTextColor(WHITE); // Réinitialisation de la couleur pour les autres éléments
        } else {
            // Élément non sélectionné
            display.setCursor(5, y_pos);
            display.print(" ");
            display.print(menuItems[i]);
        }
    }
    display.display();
}

/**
 * Gère les entrées de l'encodeur (rotation et clic) pour la navigation.
 */
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen) {
    // 1. Gestion de la rotation de l'encodeur (Navigation circulaire)
    int delta = encoder_getDelta();
    
    if (delta != 0) {
        menuIndex += delta;

        // Limiter l'index de manière circulaire
        if (menuIndex < 0) {
            menuIndex = MENU_SIZE - 1;
        } else if (menuIndex >= MENU_SIZE) {
            menuIndex = 0;
        }
    }

    // 2. Gestion du clic de l'encodeur (Sélection)
    ButtonEvent enc_event = encoder_getSwitchEvent();
    
    if (enc_event == BUTTON_SINGLE_CLICK) {
        // Change l'état de l'écran : 0 (Menu) -> 1, 2, ou 3
        *currentScreen = menuIndex + 1; 
    }
}