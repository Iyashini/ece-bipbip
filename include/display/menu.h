#pragma once
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" // Inclus pour l'événement de bouton

// Constantes menu
extern const int MENU_SIZE;

// État global minimal du menu
extern int menuIndex;

// Affichage du menu principal
void afficherMenu(Adafruit_SSD1306 &display);

// Gère les entrées (encodeur rotation et clic) pour la navigation dans le menu.
// *currentScreen est un pointeur vers la variable d'état de l'écran (dans main.cpp)
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen); // <-- CETTE LIGNE MANQUAIS