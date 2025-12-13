#pragma once
#include <Adafruit_SSD1306.h>

// Constantes menu
extern const int MENU_SIZE;

// État global minimal du menu
extern int menuIndex;

// Affichage du menu principal
void afficherMenu(Adafruit_SSD1306 &display);
