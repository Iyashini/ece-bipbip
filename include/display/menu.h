#pragma once
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" 

extern const char* menuItems[]; 
extern const int MENU_SIZE;
extern int menuIndex;

void afficherMenu(Adafruit_SSD1306 &display); 
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen);

// L'écran 2 (Messages reçus) est laissé ici pour l'instant.
void screen_inbox(Adafruit_SSD1306 &display, int *currentScreen); 
// screen_settings RETIRÉ de ce header.