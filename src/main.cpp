#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Modules d'affichage
#include "display/animation.h" 
#include "display/menu.h"      

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

// CORRECTION CRITIQUE : La LED Verte est sur A0, pas D7.
const int PIN_LED_VERTE = A0; 
const int PIN_BUZZER = 10; 

// Pins NRF24 (D7 et D8)
const int PIN_RF24_CE = 7; 
const int PIN_RF24_CSN = 8; 


// =============================
//   Écran OLED SSD1306
// =============================
Adafruit_SSD1306 display(128, 64, &Wire);

// État interface : 0 = Menu Principal
int currentScreen = 0;    

// Contexte pour le bouton externe (A6)
static ButtonContext* external_button = NULL; 


// =============================
//   SETUP GLOBAL
// =============================
void setup() {
    // Initialisation I2C
    Wire.begin(); 
    
    // CORRECTION CRITIQUE : Utilisation de l'adresse I2C 0x3C (confirmée par votre code fonctionnel)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    
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
    
    // Ajout du pinMode pour A0, maintenant que nous avons résolu le conflit D7.
    pinMode(PIN_LED_VERTE, OUTPUT);
    
    pinMode(PIN_BUZZER, OUTPUT);
    
    // NRF24 pins (D7, D8) sont laissées sans pinMode pour être gérées par la lib RF24.
    // Votre code fonctionnel les met en OUTPUT, mais ce n'est pas nécessaire ici.
    // pinMode(PIN_RF24_CE, OUTPUT);
    // pinMode(PIN_RF24_CSN, OUTPUT);

    // Initialisation des modules d'entrée
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW); 
    external_button = button_create(PIN_BUTTON);    
    
    // AFFICHAGE : Animation de Démarrage
    animationDemarrage(display); 
    delay(300);
    // AFFICHAGE : Menu Principal
    afficherMenu(display); 
}

// =============================
//   BOUCLE PRINCIPALE
// =============================
void loop() {
    // 1. Mise à jour des entrées
    encoder_update(); 
    if (external_button != NULL) {
        button_update(external_button); 
    }

    // 2. Machine à états (seul l'écran 0 est actif pour l'instant)
    switch (currentScreen) {
        case 0:
            menu_handleInput(display, &currentScreen);
            afficherMenu(display); 
            break;
        
        default:
            currentScreen = 0;
            afficherMenu(display);
            break;
    }
}