#include "input/encoder.h"
#include <Arduino.h>

// Broches définies par main.cpp
static int PIN_A, PIN_B, PIN_SW;

// État de l'encodeur
static volatile int encoder_delta = 0;
static volatile bool click_flag = false;
static int last_state_A = HIGH;

// Détection de rotation (Interruption)
void encoder_ISR() {
    int new_state_A = digitalRead(PIN_A);

    // Détection du front montant/descendant et détermination du sens
    if (new_state_A != last_state_A) {
        // La broche A a changé, on lit la broche B
        int state_B = digitalRead(PIN_B);

        if (state_B != new_state_A) {
            // A est en avance sur B : Rotation horaire (+)
            encoder_delta++;
        } else {
            // B est en avance sur A : Rotation anti-horaire (-)
            encoder_delta--;
        }
        last_state_A = new_state_A;
    }
}

// Détection de clic (Interruption)
void switch_ISR() {
    // On détecte un front descendant (appui)
    if (digitalRead(PIN_SW) == LOW) {
        click_flag = true;
    }
    // Note: L'anti-rebond sera géré dans encoder_update() (appelé dans loop())
}

void encoder_init(int pinA, int pinB, int pinSW) {
    PIN_A = pinA;
    PIN_B = pinB;
    PIN_SW = pinSW;

    // Les pinModes sont déjà définis dans setup() de main.cpp

    // Initialisation des interruptions
    // PIN_A (D3) est attachée à INTERRUPT 1
    // PIN_SW (D2) est attachée à INTERRUPT 0
    attachInterrupt(digitalPinToInterrupt(PIN_A), encoder_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SW), switch_ISR, FALLING);
}

void encoder_update() {
    // Cette fonction est réservée pour l'anti-rebond du bouton de l'encodeur si nécessaire
    // et pour le reset d'un long press si jamais il était implémenté ici.
    // Dans cette implémentation simple, elle n'a pas d'effet pour l'instant.
}

int encoder_getDelta() {
    // Copie thread-safe du delta pour éviter la corruption due à l'interruption
    noInterrupts();
    int delta = encoder_delta;
    encoder_delta = 0;
    interrupts();

    return delta;
}

bool encoder_wasClicked() {
    // Copie thread-safe du flag de clic
    noInterrupts();
    bool clicked = click_flag;
    click_flag = false;
    interrupts();

    return clicked;
}