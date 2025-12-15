#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Modules d'affichage essentiels
#include "display/animation.h" // Maintenant corrigé
#include "display/menu.h"      // Maintenant corrigé
#include "display/send_message.h" // Maintenant corrigé
#include "display/settings.h"    // Maintenant corrigé

// Modules d'entrée
#include "input/encoder.h" 
#include "input/button.h" 

// =========================
//   Définition des pins
// =========================
const int PIN_ENC_A  = 3;   
const int PIN_ENC_B  = 4;   
const int PIN_ENC_SW = 2;   
const int PIN_BUTTON = A6;  // Bouton SW3 (Externe)

const int PIN_LED_R = 5;
const int PIN_LED_G = 6;
const int PIN_LED_B = 9;

const int PIN_LED_VERTE = 7;
const int PIN_BUZZER = 10; 

const int PIN_RF24_CE = 7;  // D7
const int PIN_RF24_CSN = 8; // D8


// =============================
//   Écran OLED SSD1306
// =============================
Adafruit_SSD1306 display(128, 64, &Wire);

// État interface : 0 = Menu Principal
int currentScreen = 0;

// État du menu (pour les sous-menus dans les paramètres)
int currentMenu = 0;
int lastScreen = -1;

// Contexte pour le bouton externe (A6)
static ButtonContext* external_button = NULL; 




// =============================
//   SETUP GLOBAL (Appelé une seule fois)
// =============================
void setup() {
    // Initialisation Wire et Display
    Wire.begin(); 
    
    // Adresse I2C : Test de 0x3C
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // Échec d'initialisation : on reste bloqué pour éviter un écran noir silencieux
        pinMode(LED_BUILTIN, OUTPUT);
        while (true) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    display.clearDisplay();
    display.display();

    // Configuration des pinModes
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BUTTON, INPUT_PULLUP); 

    // PinModes des périphériques (LEDs et Buzzer)
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    
    // Suppression du pinMode pour D7/PIN_LED_VERTE pour éviter le conflit I2C/RF24/LED
    // pinMode(PIN_LED_VERTE, OUTPUT); <-- Retiré
    
    pinMode(PIN_BUZZER, OUTPUT);

    // Initialisation des modules d'entrée
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW); 
    external_button = button_create(PIN_BUTTON);
    
    send_message_init();
    settings_init();

    // Démarrage et affichage initial
    animationDemarrage(display); 
    delay(300);
    afficherMenu(display); 
}

// =============================
//   BOUCLE PRINCIPALE (Appelée sans fin)
// =============================
// Extrait de src/main.cpp - loop()

void loop() {
    // 1. Mise à jour des entrées (Polling de l'encodeur et du bouton)
    encoder_update();
    if (external_button != NULL) {
        button_update(external_button);
    }

    // 2. Gestion des transitions d'écran (détection des changements)
    if (currentScreen != lastScreen) {
        if (currentScreen == 2) {
            // Réinitialiser le sous-menu Paramètres à chaque entrée
            currentMenu = 0;
            settings_resetMenu();
        }

        lastScreen = currentScreen;
    }

    // 3. Machine à états (Appel de fonction unique par cas)
    
    switch (currentScreen) {
        case 0: // Menu Principal

            // Gère la sélection et change currentScreen
            menu_handleInput(display, &currentScreen);

            // Dessine l'écran
            afficherMenu(display);                     
            break;
        
        case 1: // Envoyer un message
            // Gère la saisie, les modes de clavier, et change currentScreen si besoin
            send_message_handleInput(&currentScreen, external_button);
            // Dessine l'écran de saisie
            send_message_drawScreen(display);
            break;

        case 2:
            if (currentMenu == -1) {
                currentScreen = 0;
                currentMenu = 0;
                afficherMenu(display);
                break;
            }

            // break;
            switch (currentMenu) {
                case 0:
                    settings_handleInput(&currentMenu, external_button);
                    settings_drawScreen(display);
                    if (currentMenu == -1) {
                        currentScreen = 0;
                        currentMenu = 0;
                        afficherMenu(display);
                    }
                break;

                case 1:
                    edit_nrf24_channel_handleInput(&currentMenu, external_button);
                    edit_nrf24_channel_drawScreen(display);
                break;

                case 2:
                    edit_ringtone_handleInput(&currentMenu, external_button);
                    edit_ringtone_drawScreen(display);
                break;

                case 3:
                    edit_pseudo_handleInput(&currentMenu, external_button);
                    edit_pseudo_drawScreen(display);
                break;
            }
            break;
    
        default:
            // Tout autre état invalide ramène au menu
            currentScreen = 0;
            afficherMenu(display);
            break;
    }
}

// switch (currentScreen) {
//         case 0: // Menu Principal
//             menu_handleInput(display, &currentScreen);
//             afficherMenu(display);                     
//             break;
        
//         case 1: // Envoyer un message
//             send_message_handleInput(&currentScreen, external_button);
//             send_message_drawScreen(display);
//             break;
            
//         // case 2: // Paramètres (Utilise le module settings dédié)
//         //     settings_handleInput(&currentScreen, external_button);
//         //     settings_drawScreen(display);
//         //     break;

//         default:
//             currentScreen = 0;
//             afficherMenu(display);
//             break;
//     }