#include <Arduino.h>
#include "input/encoder.h" 
#include "input/button.h" 

// --- Définition des Pins basées sur la conception matérielle ---
const int PIN_ENC_A  = 3;   // Encodeur A
const int PIN_ENC_B  = 4;   // Encodeur B
const int PIN_ENC_SW = 2;   // Bouton Encodeur
const int PIN_BUTTON = A6;  // Bouton SW3 (Externe)

// --- Contexte pour le bouton externe ---
static ButtonContext* external_button = NULL; 

void setup() {
    Serial.begin(9600);
    Serial.println("--- DEMARRAGE DU TEST D'ENTREES ---");

    // 1. Configuration des pinModes (Input Pull-up)
    // D3, D4, D2 sont des broches digitales
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    // A6 ne peut être utilisée qu'en analogique, mais nous la lisons en digital (INPUT_PULLUP)
    pinMode(PIN_BUTTON, INPUT_PULLUP); 

    // 2. Initialisation des modules
    // L'encodeur gère son propre bouton (D2) en créant une instance interne de ButtonContext.
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW); 
    
    // Le bouton externe (A6) nécessite sa propre instance.
    external_button = button_create(PIN_BUTTON);
}

void loop() {
    // 1. Mise à jour des Entrées (Polling)
    // Ceci met à jour la rotation et la logique de clic de l'encodeur.
    encoder_update(); 

    // Ceci met à jour la logique de clic du bouton externe (A6).
    if (external_button != NULL) {
        button_update(external_button); 
    }

    // 2. Traitement de la Rotation de l'Encodeur
    int delta = encoder_getDelta();
    if (delta > 0) {
        Serial.println("Encodeur: ROTATION HORAIRE (+)");
    } else if (delta < 0) {
        Serial.println("Encodeur: ROTATION ANTI-HORAIRE (-)");
    }

    // 3. Traitement du Bouton de l'Encodeur (D2)
    ButtonEvent enc_event = encoder_getSwitchEvent();
    switch (enc_event) {
        case BUTTON_SINGLE_CLICK:
            Serial.println("Encodeur SW: Clic Simple (SELECT)");
            break;
        case BUTTON_DOUBLE_CLICK:
            Serial.println("Encodeur SW: DOUBLE Clic (Changement Mode Clavier)");
            break;
        case BUTTON_LONG_PRESS:
            Serial.println("Encodeur SW: Clic LONG");
            break;
        default:
            break;
    }

    // 4. Traitement du Bouton Externe (A6)
    ButtonEvent btn_event = button_getEvent(external_button);
    switch (btn_event) {
        case BUTTON_SINGLE_CLICK:
            Serial.println("Bouton A6: Clic Simple");
            break;
        case BUTTON_DOUBLE_CLICK:
            Serial.println("Bouton A6: DOUBLE Clic");
            break;
        case BUTTON_LONG_PRESS:
            Serial.println("Bouton A6: Clic LONG (QUITTER/ENVOYER)");
            break;
        default:
            break;
    }

    // Ajouter un petit délai pour ne pas saturer la console
    delay(10);
}