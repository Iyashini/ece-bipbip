#pragma once
#include <Adafruit_SSD1306.h>
#include "input/button.h" 

// Le message fait 100 caractères maximum + 1 pour le terminateur '\0'
#define MAX_MESSAGE_LENGTH 101 

// Définition des modes de clavier (pour le double-clic et le clic long)
enum KeyboardMode {
    KB_AZERTY_LOWER = 0,    // azerty minuscule
    KB_AZERTY_UPPER,        // AZERTY MAJUSCULE
    KB_SYMBOLS_ACCENTS,     // Symboles + accents
    KB_NUMBERS,             // Chiffres
    KB_MODE_COUNT           // Compteur de modes
};

void send_message_init();
void send_message_drawScreen(Adafruit_SSD1306 &display);
void send_message_handleInput(int *currentScreen, ButtonContext* external_button);