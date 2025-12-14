#include "input/button.h"
#include <Arduino.h>
#include <stdlib.h> // Pour malloc/free

// --- Constantes de temps ---
#define DEBOUNCE_DELAY      50UL    // ms
#define DOUBLE_CLICK_DELAY  200UL   // ms
#define LONG_PRESS_DELAY    1000UL  // ms

// Définition interne de la structure de contexte
struct ButtonContext {
    int pin;
    
    // Variables de la machine à états
    bool last_button_state;
    bool current_button_state;
    unsigned long last_debounce_time;
    unsigned long press_start_time;
    unsigned long last_click_time;
    ButtonEvent event_detected;
};


ButtonContext* button_create(int pin) {
    ButtonContext* context = (ButtonContext*)malloc(sizeof(ButtonContext));

    if (context == NULL) {
        return NULL; // Échec de l'allocation
    }

    // Initialisation de l'état
    context->pin = pin;
    context->last_button_state = HIGH;
    context->current_button_state = HIGH;
    context->last_debounce_time = 0;
    context->press_start_time = 0;
    context->last_click_time = 0;
    context->event_detected = BUTTON_NONE;
    
    return context;
}

void button_update(ButtonContext* context) {
    if (context == NULL) return;

    int reading = digitalRead(context->pin);

    // 1. Anti-rebond (Debounce)
    if (reading != context->last_button_state) {
        context->last_debounce_time = millis();
    }

    if ((millis() - context->last_debounce_time) > DEBOUNCE_DELAY) {
        // Le nouvel état est stable
        if (reading != context->current_button_state) {
            context->current_button_state = reading;

            if (context->current_button_state == LOW) {
                // Début d'appui stable
                context->press_start_time = millis();

            } else {
                // Relâchement stable
                unsigned long press_duration = millis() - context->press_start_time;
                
                // Si l'événement long press n'a pas été détecté (durée < LONG_PRESS_DELAY)
                if (press_duration < LONG_PRESS_DELAY) {
                    
                    if ((millis() - context->last_click_time) < DOUBLE_CLICK_DELAY) {
                        // Double-clic
                        context->event_detected = BUTTON_DOUBLE_CLICK;
                        context->last_click_time = 0; 

                    } else {
                        // Simple clic (ou premier clic)
                        context->last_click_time = millis();
                        context->event_detected = BUTTON_SINGLE_CLICK;
                    }
                }
            }
        }
    }

    // 2. Détection de la pression longue
    if (context->current_button_state == LOW && context->press_start_time != 0) {
        if ((millis() - context->press_start_time) >= LONG_PRESS_DELAY) {
            if (context->event_detected != BUTTON_LONG_PRESS) {
                context->event_detected = BUTTON_LONG_PRESS;
                // Empêcher la détection d'un clic court/double après un long press
                context->last_click_time = 0; 
            }
        }
    }

    context->last_button_state = reading;
}

ButtonEvent button_getEvent(ButtonContext* context) {
    if (context == NULL) return BUTTON_NONE;
    ButtonEvent current_event = context->event_detected;
    context->event_detected = BUTTON_NONE; // Consommer l'événement
    return current_event;
}

void button_destroy(ButtonContext* context) {
    if (context != NULL) {
        free(context);
    }
}