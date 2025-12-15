#include "display/settings.h"
#include "input/encoder.h"
#include <Arduino.h>
#include <string.h>

// --- Variables d'état Globales (Implémentation) ---
// Ces valeurs sont les valeurs de configuration de l'appareil
int current_channel = 76; // Canal NRF24 par défaut
int current_ringtone = 0;   // Sonnerie par défaut (index 0)
char current_pseudo[MAX_PSEUDO_LENGTH + 1] = "BIPBIP";



// --- Variables internes au module settings ---


enum SettingsState {
    STATE_IDLE,             // Navigation dans le menu des paramètres
    STATE_EDITING_CHANNEL,  // Édition du canal NRF24
    STATE_EDITING_RINGTONE, // Édition de la sonnerie
    STATE_EDITING_PSEUDO_CHAR // Placeholder (non implémenté)
};

// Noms des sonneries disponibles
const char* ringtone_names[] = {
    "Classique", 
    "Alerte", 
    "Silence"
};
//const int MAX_RINGTONES = sizeof(ringtone_names) / sizeof(ringtone_names[0]);

// Noms des éléments du menu de paramètres
const char* setting_names[] = {
    "Canal NRF24",
    "Sonnerie",
    "Pseudo",
    "Retour"
};
const int SETTINGS_ITEM_COUNT = sizeof(setting_names) / sizeof(setting_names[0]);
int setting_menuIndex = 0;

void settings_init() {
    // Initialisation. Pour l'instant, les variables globales ont des valeurs par défaut
    // (Dans une version finale, on lirait ici la mémoire EEPROM).

}

void settings_resetMenu() {
    setting_menuIndex = 0;
}

void settings_drawScreen(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    for (int i = 0; i < SETTINGS_ITEM_COUNT; i++) {
        display.setCursor(0, i * 15);
        
        if (i == setting_menuIndex) {
            display.setTextColor(BLACK, WHITE); 
            display.print(">");
            display.print(setting_names[i]);
            display.setTextColor(WHITE);
        } else {
            display.print(" ");
            display.print(setting_names[i]);
        }
    }
    display.display();
}

void settings_handleInput(int *currentMenu, ButtonContext* external_button) {
  const int delta = encoder_getDelta();
  const ButtonEvent enc_event = encoder_getSwitchEvent();
  const ButtonEvent btn_event = (external_button != NULL) ? button_getEvent(external_button) : BUTTON_NONE;

  // Long press = retour menu principal
  if (btn_event == BUTTON_LONG_PRESS) {
    *currentMenu = -1;
    settings_resetMenu();
    return;
  }

  // Navigation uniquement si on est sur la liste
  if (*currentMenu != 0) return;

  if (delta != 0) {
    setting_menuIndex += delta;
    if (setting_menuIndex < 0) setting_menuIndex = SETTINGS_ITEM_COUNT - 1;
    if (setting_menuIndex >= SETTINGS_ITEM_COUNT) setting_menuIndex = 0;
  }

  if (enc_event == BUTTON_SINGLE_CLICK) {
    if (setting_menuIndex == SETTINGS_ITEM_COUNT - 1) {
      // Dernier élément : retour au menu principal
      *currentMenu = -1;
      settings_resetMenu();
    } else {
      // 0->1, 1->2, 2->3
      *currentMenu = setting_menuIndex + 1;
    }
  }
}


// Page pour éditer le canal NRF24
void edit_nrf24_channel_drawScreen(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Editer Canal NRF24");
    display.setCursor(0, 20);
    display.print("Canal: ");
    display.print(current_channel);
    display.display();
}

void edit_nrf24_channel_handleInput(int *currentMenu, ButtonContext* external_button) {
    int delta = encoder_getDelta();
    ButtonEvent enc_event = encoder_getSwitchEvent();
    ButtonEvent btn_event = (external_button != NULL) ? button_getEvent(external_button) : BUTTON_NONE;
    
    if (delta != 0) {
        current_channel += delta;

        if (current_channel < MIN_CHANNEL_VALUE) {
            current_channel = MIN_CHANNEL_VALUE;
        } else if (current_channel > MAX_CHANNEL_VALUE) {
            current_channel = MAX_CHANNEL_VALUE;
        }
    }

    if (btn_event == BUTTON_LONG_PRESS || enc_event == BUTTON_SINGLE_CLICK) {
        *currentMenu = 0;
        settings_resetMenu();
    }
}

// Page pour éditer la sonnerie
void edit_ringtone_drawScreen(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Editer Sonnerie");
    display.setCursor(0, 20);
    display.print("Sonnerie: ");
    display.print(ringtone_names[current_ringtone]);
    display.display();
}

void edit_ringtone_handleInput(int *currentMenu, ButtonContext* external_button) {
    int delta = encoder_getDelta();
    ButtonEvent enc_event = encoder_getSwitchEvent();
    ButtonEvent btn_event = (external_button != NULL) ? button_getEvent(external_button) : BUTTON_NONE;
    
    if (delta != 0) {
        current_ringtone += delta;

        if (current_ringtone < 0) {
            current_ringtone = 0;
        } else if (current_ringtone >= MAX_RINGTONES) {
            current_ringtone = MAX_RINGTONES - 1;
        }
    }

    if (enc_event == BUTTON_SINGLE_CLICK || btn_event == BUTTON_LONG_PRESS) {
        *currentMenu = 0;
        settings_resetMenu();
    }
}

// Page pour éditer le pseudo (non implémentée)
void edit_pseudo_drawScreen(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Editer Pseudo");
    display.setCursor(0, 20);
    display.print("Pseudo: ");
    display.print(current_pseudo);
    display.display();
}

void edit_pseudo_handleInput(int *currentMenu, ButtonContext* external_button) {
    ButtonEvent enc_event = encoder_getSwitchEvent();
    ButtonEvent btn_event = (external_button != NULL) ? button_getEvent(external_button) : BUTTON_NONE;

    if (enc_event == BUTTON_SINGLE_CLICK || btn_event == BUTTON_LONG_PRESS) {
        *currentMenu = 0;
        settings_resetMenu();
    }
}
