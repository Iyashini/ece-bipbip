#pragma once
#include <stdbool.h>

// Événements détectables pour tous les boutons
enum ButtonEvent {
    BUTTON_NONE = 0,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS
};

// Structure de contexte pour rendre le module réutilisable
typedef struct ButtonContext ButtonContext;

// Initialise un nouveau contexte de bouton pour une broche donnée.
// Le pin doit être configuré en INPUT_PULLUP AVANT l'appel.
ButtonContext* button_create(int pin);

// Met à jour l'état d'un bouton (doit être appelée dans loop() pour le polling).
void button_update(ButtonContext* context);

// Renvoie l’événement détecté depuis le dernier appel.
ButtonEvent button_getEvent(ButtonContext* context);

// Libère la mémoire allouée (bonne pratique, mais non critique sur Arduino si l'appel est unique)
void button_destroy(ButtonContext* context);