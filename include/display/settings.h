#pragma once
#include <Adafruit_SSD1306.h>
#include "input/encoder.h"
#include "input/button.h"
#include <stdint.h> // Pour uint8_t

// Constantes pour les limites
#define MAX_CHANNEL_VALUE 127
#define MIN_CHANNEL_VALUE 0
#define MAX_RINGTONES 3
#define MAX_PSEUDO_LENGTH 10

// Définition des éléments du menu de paramètres
enum SettingsItem {
    SETTING_CHANNEL = 0,
    SETTING_RINGTONE,
    SETTING_PSEUDO,
    // SETTINGS_ITEM_COUNT,
    SETTING_BACK, // Option de retour au menu principal

};


// Variables d'état global (simulent un stockage persistant)
// Elles sont 'extern' pour être accessibles par d'autres modules
extern int current_channel;
extern int current_ringtone;
extern char current_pseudo[MAX_PSEUDO_LENGTH + 1]; // +1 pour le terminateur '\0'

// Prototypes des fonctions
void settings_init();
void settings_resetMenu();
void settings_handleInput(int *currentScreen, ButtonContext* external_button);
void settings_drawScreen(Adafruit_SSD1306 &display);
void edit_nrf24_channel_drawScreen(Adafruit_SSD1306 &display);
void edit_ringtone_drawScreen(Adafruit_SSD1306 &display);
void edit_pseudo_drawScreen(Adafruit_SSD1306 &display);

void edit_nrf24_channel_handleInput(int *currentScreen, ButtonContext* external_button);
void edit_ringtone_handleInput(int *currentScreen, ButtonContext* external_button);
void edit_pseudo_handleInput(int *currentScreen, ButtonContext* external_button);
