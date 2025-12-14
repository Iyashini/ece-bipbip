#include "display/send_message.h"
#include "input/encoder.h"
#include <Arduino.h>
#include <string.h> 

// Caractère de suppression (à afficher dans le clavier)
const char DELETE_CHAR = 127; 
const char SPACE_CHAR = ' ';

// --- Définition des 4 modes de clavier ---
// Le caractère ' ' (Espace) est omis ici car géré par le bouton A6.
// Le caractère 127 (DELETE_CHAR) est le premier élément pour une sélection rapide.

const char* KB_CHARS[KB_MODE_COUNT] = {
    // 0. Minuscule (AZERTY)
    "\x7F" "azertyuiopqsdfghjklmwxcvbn", 
    // 1. MAJUSCULE (AZERTY)
    "\x7F" "AZERTYUIOPQSDFGHJKLMWXCVBN", 
    // 2. Symboles & Accents
    "\x7F" ".,?!:;\'\"()[]-+=*/&@#%$€£çàéèêëïîôöùûüñ",
    // 3. Chiffres
    "\x7F" "0123456789"
};

// --- Variables d'état ---
static char messageBuffer[MAX_MESSAGE_LENGTH];
static int cursorIndex = 0;             
static int charIndexInMode = 1;         // Index du caractère sélectionné (commence après DELETE_CHAR)
static enum KeyboardMode currentMode = KB_AZERTY_LOWER;


void send_message_init() {
    memset(messageBuffer, 0, MAX_MESSAGE_LENGTH); 
    messageBuffer[0] = '\0'; 
    cursorIndex = 0;
    charIndexInMode = 1; 
    currentMode = KB_AZERTY_LOWER;
}


void send_message_drawScreen(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, WHITE);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // 1. ZONE DE TEXTE (2 lignes)
    display.setCursor(5, 5);
    display.print("MSG: ");
    
    // Affichage du message avec défilement si nécessaire (Zone de 20 caractères)
    int start_char = 0;
    int visible_len = 20; 
    
    if (cursorIndex > visible_len) {
        start_char = cursorIndex - visible_len; 
    }
    
    for(int i = 0; i < visible_len && messageBuffer[start_char + i] != '\0'; i++) {
        display.print(messageBuffer[start_char + i]);
    }
    
    // Curseur de saisie (petit rectangle clignotant)
    if ((millis() % 1000) < 500) {
        // La position du curseur est basée sur le nombre de caractères affichés
        int current_pos = 5 + (cursorIndex - start_char) * 6; // 6 pixels par caractère
        display.fillRect(current_pos, 13, 5, 2, WHITE);
    }
    
    // 2. Ligne de statut (Curseur/Mode)
    display.setCursor(5, 25);
    display.print("Mode: ");
    // CORRECTION W-switch: Ajout d'un default pour gérer KB_MODE_COUNT
    switch (currentMode) {
        case KB_AZERTY_LOWER: display.print("AZERTY min"); break;
        case KB_AZERTY_UPPER: display.print("AZERTY MAJ"); break;
        case KB_SYMBOLS_ACCENTS: display.print("SYMBOLES"); break;
        case KB_NUMBERS: display.print("CHIFFRES"); break;
        default: display.print("ERREUR MODE"); break; 
    }
    display.print(" (");
    display.print(cursorIndex);
    display.print("/");
    display.print(MAX_MESSAGE_LENGTH - 1);
    display.print(")");


    // 3. CLAVIER (Affiche la sélection courante)
    const char* currentKB = KB_CHARS[currentMode];
    
    display.setTextSize(2); 
    display.setCursor(30, 40); // Centré
    
    // Affichage des chevrons
    display.print(">");
    
    // Si c'est le caractère de suppression
    if (currentKB[charIndexInMode] == DELETE_CHAR) {
        display.print("DEL");
    } else {
        display.print(currentKB[charIndexInMode]);
    }
    display.print("<");

    display.display();
}


void send_message_handleInput(int *currentScreen, ButtonContext* external_button) {
    const char* currentKB = KB_CHARS[currentMode];
    size_t modeLength = strlen(currentKB); // Unsigned size_t

    // --- 1. Gestion de la rotation (Sélection du caractère) ---
    int delta = encoder_getDelta();
    if (delta != 0) {
        charIndexInMode += delta;

        // Le modulo doit inclure l'index 0 (DELETE_CHAR)
        if (charIndexInMode < 0) {
            // Pas de warning ici car 0 est comparé à charIndexInMode (signed)
            charIndexInMode = (int)modeLength - 1; 
        } 
        // CORRECTION W-sign-compare : Cast de modeLength à int
        else if (charIndexInMode >= (int)modeLength) { 
            charIndexInMode = 0;
        }
    }

    // --- 2. Gestion du clic de l'encodeur ---
    ButtonEvent enc_event = encoder_getSwitchEvent();

    if (enc_event == BUTTON_SINGLE_CLICK) {
        // Clic simple : Saisie du caractère
        char selectedChar = currentKB[charIndexInMode];
        
        if (selectedChar == DELETE_CHAR) {
            // ACTION: Supprimer le dernier caractère
            if (cursorIndex > 0) {
                cursorIndex--;
                messageBuffer[cursorIndex] = '\0';
            }
        } 
        else if (cursorIndex < MAX_MESSAGE_LENGTH - 1) {
            // ACTION: Saisir le caractère
            messageBuffer[cursorIndex] = selectedChar;
            cursorIndex++;
            messageBuffer[cursorIndex] = '\0'; // Terminateur
        }
        
        // Réinitialise la sélection pour la fluidité
        charIndexInMode = 1; 

    } else if (enc_event == BUTTON_LONG_PRESS) {
        // Clic Long Encodeur : Bascule MAJ/MINUSCULE
        if (currentMode == KB_AZERTY_LOWER) {
            currentMode = KB_AZERTY_UPPER;
        } else if (currentMode == KB_AZERTY_UPPER) {
            currentMode = KB_AZERTY_LOWER;
        }
        charIndexInMode = 1; // Commence après DEL
        
    } else if (enc_event == BUTTON_DOUBLE_CLICK) {
        // Double Clic Encodeur : Rotation des claviers 
        
        if (currentMode == KB_AZERTY_LOWER || currentMode == KB_AZERTY_UPPER) {
            currentMode = KB_SYMBOLS_ACCENTS;
        } else if (currentMode == KB_SYMBOLS_ACCENTS) {
            currentMode = KB_NUMBERS;
        } else if (currentMode == KB_NUMBERS) {
            currentMode = KB_AZERTY_LOWER;
        }
        
        charIndexInMode = 1;
    } 

    // --- 3. Gestion du bouton externe (Espace / Quitter/Envoyer) ---
    ButtonEvent btn_event = button_getEvent(external_button);

    if (btn_event == BUTTON_SINGLE_CLICK) {
        // Clic simple A6 : Ajout d'un Espace
        if (cursorIndex < MAX_MESSAGE_LENGTH - 1) {
            messageBuffer[cursorIndex] = SPACE_CHAR;
            cursorIndex++;
            messageBuffer[cursorIndex] = '\0'; 
        }
    } else if (btn_event == BUTTON_LONG_PRESS) {
        // Clic Long A6 : Quitter l'édition pour revenir au menu
        *currentScreen = 0; 
        send_message_init();
        return;
    }
}