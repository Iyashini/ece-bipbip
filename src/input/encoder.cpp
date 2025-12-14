#include "input/encoder.h"
#include <Arduino.h>

// Broches définies par main.cpp
static int PIN_A, PIN_B;
static ButtonContext* switch_context = NULL; // Contexte pour le bouton de l'encodeur (D2)

// État de l'encodeur
static volatile int encoder_delta = 0;
static int last_state_A = HIGH;

// Détection de rotation (Interruption)
void encoder_ISR() {
    // La logique de rotation (D3/D4) reste inchangée
    int new_state_A = digitalRead(PIN_A);
    if (new_state_A != last_state_A) {
        int state_B = digitalRead(PIN_B);
        if (state_B != new_state_A) {
            encoder_delta++; // Rotation horaire
        } else {
            encoder_delta--; // Rotation anti-horaire
        }
        last_state_A = new_state_A;
    }
}

void encoder_init(int pinA, int pinB, int pinSW) {
    PIN_A = pinA;
    PIN_B = pinB;

    // Création du contexte de bouton pour l'interrupteur de l'encodeur (D2)
    switch_context = button_create(pinSW); 
    
    // Initialisation des interruptions pour la ROTATION SEULEMENT (PIN_A sur D3)
    attachInterrupt(digitalPinToInterrupt(PIN_A), encoder_ISR, CHANGE);
}

void encoder_update() {
    // Appel au polling du bouton pour gérer le clic complexe (simple, double, long)
    if (switch_context != NULL) {
        button_update(switch_context);
    }
}

int encoder_getDelta() {
    // Copie thread-safe du delta
    noInterrupts();
    int delta = encoder_delta;
    encoder_delta = 0;
    interrupts();
    return delta;
}

ButtonEvent encoder_getSwitchEvent() {
    if (switch_context != NULL) {
        return button_getEvent(switch_context);
    }
    return BUTTON_NONE;
}