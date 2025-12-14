#include "input/button.h"
#include <Arduino.h>

// --- Constantes de temps ---
#define DEBOUNCE_DELAY      50UL    // Anti-rebond (50 ms)
#define DOUBLE_CLICK_DELAY  300UL   // Fenêtre d'attente pour le double-clic (200 ms)
#define LONG_PRESS_DELAY    600UL  // Déclenchement du clic long (1 seconde)

// Seuil pour la lecture analogique (A6)
#define ANALOG_THRESHOLD    50 

// Définition interne de la structure de contexte
struct ButtonContextInternal {
    int pin;
    
    // Variables de la machine à états
    bool last_button_state;
    bool current_button_state;
    unsigned long last_debounce_time;
    unsigned long press_start_time;
    unsigned long last_click_time; // Temps du dernier relâchement (départ de la fenêtre DOUBLE_CLICK)
    ButtonEvent event_detected;
};

// POOL STATIQUE : Allocation de mémoire fixe (pas de malloc)
#define MAX_BUTTONS 4
static struct ButtonContextInternal button_pool[MAX_BUTTONS];
static int next_free_index = 0;


ButtonContext* button_create(int pin) {
    if (next_free_index >= MAX_BUTTONS) return NULL;

    struct ButtonContextInternal* context = &button_pool[next_free_index];
    next_free_index++;

    // Initialisation
    context->pin = pin;
    context->last_button_state = HIGH;
    context->current_button_state = HIGH;
    context->last_debounce_time = 0;
    context->press_start_time = 0;
    context->last_click_time = 0;
    context->event_detected = BUTTON_NONE;
    
    return (ButtonContext*)context;
}

// button_destroy (maintien pour l'interface)
void button_destroy(ButtonContext* context) {
    (void)context; 
}


// Lecture adaptée de la broche (Correction A6)
static int read_button_state(int pin) {
    // Lecture analogique nécessaire pour A6 (ou autres pins A0-A7)
    if (pin >= A0 && pin <= A7) { 
        return (analogRead(pin) < ANALOG_THRESHOLD) ? LOW : HIGH;
    } else {
        // Lecture numérique standard
        return digitalRead(pin);
    }
}

void button_update(ButtonContext* context_ptr) {
    struct ButtonContextInternal* context = (struct ButtonContextInternal*)context_ptr;
    if (context == NULL) return;

    // A. Génération du SINGLE_CLICK retardé si la fenêtre de DOUBLE_CLICK a expiré
    if (context->event_detected == BUTTON_NONE && context->last_click_time != 0) {
        if ((millis() - context->last_click_time) > DOUBLE_CLICK_DELAY) {
            // La fenêtre est expirée (200ms écoulées) : on confirme l'événement single click
            context->event_detected = BUTTON_SINGLE_CLICK;
            context->last_click_time = 0; // Réinitialise l'attente
        }
    }

    int reading = read_button_state(context->pin);

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
                
                // Si l'événement long press n'a pas été détecté
                if (press_duration < LONG_PRESS_DELAY) {
                    
                    if (context->last_click_time != 0) {
                         // 2ème clic potentiel (dans la fenêtre de 200ms)
                        if ((millis() - context->last_click_time) < DOUBLE_CLICK_DELAY) {
                            // Clic double détecté
                            context->event_detected = BUTTON_DOUBLE_CLICK;
                            context->last_click_time = 0; // Double clic consomme le temps
                        } else {
                            // 2ème clic mais trop tard (la fenêtre est expirée)
                            // Le SINGLE_CLICK du 1er clic a déjà été généré en A.
                            context->last_click_time = millis(); // Redémarrer l'attente pour un nouveau double clic
                        }
                        
                    } else {
                        // 1er clic : Démarrer l'attente
                        context->last_click_time = millis();
                        // IMPORTANT : Ne génère PAS d'événement ici, on attend la confirmation en A.
                    }
                }
            }
        }
    }

    // B. Détection de la pression longue
    if (context->current_button_state == LOW && context->press_start_time != 0) {
        if ((millis() - context->press_start_time) >= LONG_PRESS_DELAY) {
            if (context->event_detected != BUTTON_LONG_PRESS) {
                context->event_detected = BUTTON_LONG_PRESS;
                context->last_click_time = 0; // Annule l'attente de clic simple
            }
        }
    }

    context->last_button_state = reading;
}

ButtonEvent button_getEvent(ButtonContext* context_ptr) {
    struct ButtonContextInternal* context = (struct ButtonContextInternal*)context_ptr;
    if (context == NULL) return BUTTON_NONE;
    
    ButtonEvent current_event = context->event_detected;
    context->event_detected = BUTTON_NONE; // Consommer l'événement
    
    return current_event;
}