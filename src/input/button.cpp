#include "input/button.h"
#include <Arduino.h>

// --- Constantes de temps ---
// Doivent être définies ici pour être accessibles par la fonction d'implémentation
#define DEBOUNCE_DELAY      50      // ms
#define DOUBLE_CLICK_DELAY  200     // ms
#define LONG_PRESS_DELAY    1000    // ms

// --- Variables d'état par bouton ---

// Un structure ou une classe serait préférable, mais pour la simplicité et
// puisque nous n'avons qu'un seul bouton (A6), nous utilisons des statics
// pour le moment. Nous allons adapter pour être générique si le besoin se présente.
// Pour l'instant, on implémente pour le bouton A6.

static int button_pin = -1;

static bool last_button_state = HIGH;
static bool current_button_state = HIGH;

static unsigned long last_debounce_time = 0;
static unsigned long press_start_time = 0;
static unsigned long last_click_time = 0;

static ButtonEvent event_detected = BUTTON_NONE;

void button_init(int pin) {
    button_pin = pin;
    // Le pinMode est déjà configuré en INPUT_PULLUP dans main.cpp
}

void button_update() {
    if (button_pin == -1) return;

    // 1. Lecture de l'état physique du bouton (LOW = appuyé)
    int reading = digitalRead(button_pin);

    // 2. Anti-rebond (Debounce)
    if (reading != last_button_state) {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) {
        // Le nouvel état est stable
        if (reading != current_button_state) {
            current_button_state = reading;

            if (current_button_state == LOW) {
                // --- Début d'appui stable ---
                press_start_time = millis();

            } else {
                // --- Relâchement stable ---
                unsigned long press_duration = millis() - press_start_time;

                if (press_duration < LONG_PRESS_DELAY) {
                    // Clic court ou double-clic

                    if ((millis() - last_click_time) < DOUBLE_CLICK_DELAY) {
                        // Clic dans la fenêtre du double-clic
                        event_detected = BUTTON_DOUBLE_CLICK;
                        last_click_time = 0; // Réinitialiser pour éviter un triple-clic
                    } else {
                        // Premier clic d'une séquence ou simple clic
                        last_click_time = millis();
                        event_detected = BUTTON_SINGLE_CLICK;
                    }
                }
                // Si la durée est > LONG_PRESS_DELAY, l'événement long press a déjà été détecté
                // et l'événement single/double click n'est pas généré.
            }
        }
    }

    // 3. Détection de la pression longue (doit être checké pendant l'appui)
    if (current_button_state == LOW && press_start_time != 0) {
        if ((millis() - press_start_time) >= LONG_PRESS_DELAY) {
            if (event_detected != BUTTON_LONG_PRESS) {
                event_detected = BUTTON_LONG_PRESS;
                // Empêcher la détection d'un double-clic après un long press
                last_click_time = 0; 
            }
        }
    }

    last_button_state = reading;
}

ButtonEvent button_getEvent() {
    ButtonEvent current_event = event_detected;
    event_detected = BUTTON_NONE; // Consommer l'événement
    return current_event;
}