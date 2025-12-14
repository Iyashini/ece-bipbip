#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Modules d'affichage
#include "display/animation.h" 
#include "display/menu.h"      
#include "display/send_message.h" 

// Modules d'entrée
#include "input/encoder.h" 
#include "input/button.h" 

// ... (Définitions des broches restent identiques) ...
const int PIN_ENC_A = 2;  
const int PIN_ENC_B = 3;  
const int PIN_ENC_SW = 4; 
const int PIN_BUTTON = A6; 
const int PIN_LED_VERTE = A0; 
const int PIN_LED_R = 5;
const int PIN_LED_G = 6;
const int PIN_LED_B = 9;
const int PIN_BUZZER = 10; 
const int PIN_RF24_CE = 7; 
const int PIN_RF24_CSN = 8; 


// =============================
//   Écran OLED SSD1306
// =============================
#define OLED_RESET -1 
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// État interface : 0 = Menu Principal, 1 = Envoyer Message
int currentScreen = 0;    

// Contexte pour le bouton externe (A6)
static ButtonContext* external_button = NULL; 


// =============================
//   SETUP GLOBAL (CORRIGÉ)
// =============================
void setup() {
    Serial.begin(9600);
    
    // 1. Initialisation I2C et OLED
    Wire.begin(); 
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("Erreur SSD1306."));
        for (;;); 
    }
    display.clearDisplay();
    display.display();
    
    // 2. PinMode des broches
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BUTTON, INPUT_PULLUP); 

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_LED_VERTE, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    // 3. Initialisation des modules d'entrée
    encoder_init(PIN_ENC_A, PIN_ENC_B, PIN_ENC_SW); 
    external_button = button_create(PIN_BUTTON);    
    
    // 4. Affichage (Animation)
    animationDemarrage(display); 
    
    // CORRECTION : Déplacement de l'initialisation du module UI APRES l'animation.
    send_message_init(); 
    
    delay(300);
    afficherMenu(display); 
}

// ... (La boucle loop() reste inchangée, elle est correcte) ...
void loop() {
    // 1. Mise à jour des entrées
    encoder_update(); 
    if (external_button != NULL) {
        button_update(external_button); 
    }

    // 2. Machine à états (Gestion de l'écran actif)
    switch (currentScreen) {
        case 0:
            // Écran Menu Principal (ACCUEIL)
            menu_handleInput(display, &currentScreen);
            afficherMenu(display);                     
            break;
        
        case 1:
            // Écran "Envoyer un message" (Saisie de texte)
            send_message_handleInput(&currentScreen, external_button);
            send_message_drawScreen(display);
            break;
            
        case 2:
            // Écran "Messages reçus" (Placeholder)
            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print("2. MESSAGES RECUS (TODO)");
            display.print("\nClic pour retour");
            display.display();
             if (encoder_getSwitchEvent() == BUTTON_SINGLE_CLICK) {
                 currentScreen = 0;
             }
            break;
            
        case 3:
            // Écran "Paramètres" (Placeholder)
            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print("3. PARAMETRES (TODO)");
            display.print("\nClic pour retour");
            display.display();
             if (encoder_getSwitchEvent() == BUTTON_SINGLE_CLICK) {
                 currentScreen = 0;
             }
            break;

        default:
            // État inconnu : Retour au menu par défaut
            currentScreen = 0;
            afficherMenu(display);
            break;
    }
}