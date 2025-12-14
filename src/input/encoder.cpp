#include "input/encoder.h"
#include <Arduino.h>

static int PIN_A, PIN_B;
static ButtonContext* switch_context = NULL; 
static volatile int encoder_delta = 0;
static int last_state_A = HIGH;

// Interruption pour la détection de rotation
void encoder_ISR() {
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

    switch_context = button_create(pinSW); 
    
    // Attache l'interruption pour la rotation uniquement (D3)
    attachInterrupt(digitalPinToInterrupt(PIN_A), encoder_ISR, CHANGE);
}

void encoder_update() {
    // Polling du bouton de l'encodeur pour les clics complexes
    if (switch_context != NULL) {
        button_update(switch_context);
    }
}

int encoder_getDelta() {
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