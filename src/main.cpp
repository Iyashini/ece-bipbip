#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "display/animation.h"
#include "display/menu.h"
// NOUVEAU : Inclure les modules d'entrée
#include "input/encoder.h" 
#include "input/button.h" 

// =========================
//   Définition des pins
// =========================
const int PIN_ENC_A  = 3;
const int PIN_ENC_B  = 4;
const int PIN_ENC_SW = 2; // D2 (Interruption 0)
const int PIN_BUTTON = A6; // Bouton externe

const int PIN_LED_R = 5;
const int PIN_LED_G = 6;
const int PIN_LED_B = 9;
const int PIN_LED_VERTE = 7;

const int PIN_BUZZER = 10;

const int PIN_RF24_CE = A4;
const int PIN_RF24_CSN = A5;

// =============================
//   Écran OLED SSD1306
// =============================
Adafruit_SSD1306 display(128, 64, &Wire);

// État interface
// 0 = menu principal, 1 = Envoyer un message, 2 = Messages reçus, 3 = Paramètres
int currentScreen = 0;    

// Contexte pour le bouton externe (A6)
static ButtonContext* external_button = NULL; 

// =============================
//   Fonctions d'écran (Stubs)
// =============================

// Déclarations (les implémentations sont basiques pour l'instant)
void screenEnvoyerMessage();
void screenEnvoyerMessage_handleInput();


void screenEnvoyerMessage() {
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, WHITE);
    display.setCursor(5, 5);
    display.setTextColor(WHITE);
    display.print("Envoyer un message");
    display.setCursor(5, 20);
    display.print("TODO: Saisie de texte");
    display.display();
}

void screenEnvoyerMessage_handleInput() {
    // Placeholder pour le retour au menu principal (simple clic encodeur)
    ButtonEvent enc_event = encoder_getSwitchEvent();
    if (enc_event == BUTTON_SINGLE_CLICK) {
        currentScreen = 0; // Retour au menu principal
        // On appelle l'affichage pour rafraîchir l'écran
        afficherMenu(display); 
        return;
    }
    // Gestion du bouton externe A6
    button_getEvent(external_button); // Consommer l'événement (à utiliser plus tard pour l'envoi)
}

// =============================
//   SETUP GLOBAL
// =============================
void setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    // Configuration des pinModes
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BUTTON, INPUT_PULLUP); // PIN_BUTTON A6

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_LED_VERTE, OUTPUT);

    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_RF24_CE, OUTPUT);
    pinMode(PIN_RF24_CSN, OUTPUT);
    pinMode(10, OUTPUT);    // Broche 10 déjà définie par PIN_BUZZER (bonne pratique: utiliser PIN_BUZZER partout)

    // Initialisation des modules
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW);
    external_button = button_create(PIN_BUTTON); // Initialisation du bouton A6

    animationDemarrage(display);
    delay(300);
    afficherMenu(display);
}

// =============================
//   BOUCLE PRINCIPALE
// =============================
void loop() {
    // 1. Mise à jour des entrées (Doit être appelé à chaque boucle pour le polling)
    encoder_update(); // Met à jour la rotation et le bouton de l'encodeur (D2)
    if (external_button != NULL) {
        button_update(external_button); // Met à jour le bouton externe (A6)
    }

    // 2. Machine à états de l'écran principal
    switch (currentScreen) {
        case 0:
            // Menu principal
            menu_handleInput(display, &currentScreen); // Gère la navigation/sélection par encodeur
            break;
        
        case 1:
            // Écran "Envoyer un message"
            screenEnvoyerMessage_handleInput();
            break;
            
        case 2:
            // Écran "Messages reçus" (à implémenter)
            break;

        case 3:
            // Écran "Paramètres" (à implémenter)
            break;

        default:
            currentScreen = 0;
            afficherMenu(display);
            break;
    }
    
    // Affichage des écrans qui ne sont pas gérés par menu_handleInput
    if (currentScreen == 1) {
        screenEnvoyerMessage();
    }
    // ... (ajouter les appels aux autres écrans ici)
}