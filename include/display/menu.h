#pragma once
#include <Adafruit_SSD1306.h>
#include "input/encoder.h" 

extern const int MENU_SIZE;
extern int menuIndex;

void afficherMenu(Adafruit_SSD1306 &display); 
void menu_handleInput(Adafruit_SSD1306 &display, int *currentScreen);