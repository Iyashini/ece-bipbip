#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Modules d'affichage
#include "display/animation.h"
#include "display/menu.h"

// Modules d'entrée (fondation solide)
#include "input/encoder.h" 
#include "input/button.h" 

// =========================
//   Définition des pins
// =========================
const int PIN_ENC_A  = 3;   // Rotation (Interruption 1)
const int PIN_ENC_B  = 4;   // Rotation
const int PIN_ENC_SW = 2;   // Clic (Interruption 0, utilisé en polling)
const int PIN_BUTTON = A6;  // Bouton externe (Polling)

const int PIN_LED_R = 5;
const int PIN_LED_G = 6;
const int PIN_LED_B = 9;
const int PIN_LED_VERTE = 7;

const int PIN_BUZZER = 10; // Attention: PIN_BUZZER et PIN_RF24_CSN sont souvent différents
// Nous allons conserver la définition comme dans votre fichier :
const int PIN_RF24_CE = A4;
const int PIN_RF24_CSN = A5;


// =============================
//   Écran OLED SSD1306
// =============================
Adafruit_SSD1306 display(128, 64, &Wire);

// État interface : 
// 0 = menu principal, 1 = Envoyer un message, 2 = Messages reçus, 3 = Paramètres
int currentScreen = 0;    

// Contexte pour le bouton externe (A6)
static ButtonContext* external_button = NULL; 

// =============================
//   Écran : ENVOYER MESSAGE (Stub)
// =============================
// Affiche l'écran de saisie (sera mis à jour par le module text_input)
void screenEnvoyerMessage() {
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, WHITE);
    display.setCursor(5, 5);
    display.setTextColor(WHITE);
    display.print("1. Envoyer un message");
    display.setCursor(5, 20);
    display.print("TODO: Implementation saisie");
    display.setCursor(5, 40);
    display.print("(Clic simple: Menu)");
    display.display();
}

// Gère les entrées de l'écran de saisie
void screenEnvoyerMessage_handleInput() {
    // Le clic simple sur l'encodeur ramène au menu principal
    ButtonEvent enc_event = encoder_getSwitchEvent();
    if (enc_event == BUTTON_SINGLE_CLICK) {
        currentScreen = 0; // Retour au menu principal
        afficherMenu(display); // Redessine le menu
        return;
    }
    
    // Le clic long sur le bouton A6 pourrait être utilisé pour annuler/quitter
    ButtonEvent btn_event = button_getEvent(external_button);
    if (btn_event == BUTTON_LONG_PRESS) {
        currentScreen = 0; // Retour au menu principal
        afficherMenu(display);
        return;
    }
    // TODO: Le double clic de l'encodeur sera utilisé ici pour changer le mode clavier.
}

// =============================
//   SETUP GLOBAL
// =============================
void setup() {
    // Initialisation Wire et Display
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    // Configuration des pinModes
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BUTTON, INPUT_PULLUP); // PIN_BUTTON A6

    // PinModes des périphériques (LEDs, Buzzer, RF24)
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_LED_VERTE, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_RF24_CE, OUTPUT);
    pinMode(PIN_RF24_CSN, OUTPUT);

    // Initialisation des modules d'entrée
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW); // Rotation (Interruption) et Clic (Polling interne)
    external_button = button_create(PIN_BUTTON);    // Bouton A6 (Polling)

    // Initialisation / Démarrage
    animationDemarrage(display);
    delay(300);
    afficherMenu(display); // Affichage initial du menu