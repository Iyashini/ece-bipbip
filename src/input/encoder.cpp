#include "input/encoder.h"
#include <Arduino.h>

static int PIN_A, PIN_B;
static ButtonContext* switch_context = NULL; 
static volatile int encoder_delta = 0;
static int last_state_A = HIGH;

// Interruption pour la détection de rotation
void encoder_ISR() {
    // Lecture de l'état actuel de la broche A
    int new_state_A = digitalRead(PIN_A);

    // Vérifie qu'il y a eu un changement d'état réel sur A (anti-rebond minimal)
    if (new_state_A != last_state_A) { 
        
        // CORRECTION DU DOUBLE-COMPTE : 
        // Nous ne comptons la rotation que sur le front montant (A passe à HIGH).
        if (new_state_A == HIGH) {
            int state_B = digitalRead(PIN_B);
            
            // Détermine la direction
            if (state_B != new_state_A) { // Si B est différent de A (donc B est LOW)
                encoder_delta++; // Rotation horaire
            } else { // Si B est égal à A (donc B est HIGH)
                encoder_delta--; // Rotation anti-horaire
            }
        }
        
        // Met à jour le dernier état pour la prochaine comparaison
        last_state_A = new_state_A;
    }
}

void encoder_init(int pinA, int pinB, int pinSW) {
    PIN_A = pinA;
    PIN_B = pinB;

    switch_context = button_create(pinSW); 
    
    // L'interruption doit rester sur CHANGE pour détecter les deux fronts, 
    // car le filtrage est géré à l'intérieur de l'ISR.
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