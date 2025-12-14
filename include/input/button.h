#pragma once
#include <stdbool.h>

enum ButtonEvent {
    BUTTON_NONE = 0,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS
};

typedef struct ButtonContext ButtonContext;

// Crée une instance de bouton (multi-instance)
ButtonContext* button_create(int pin);

// Doit être appelée dans loop() pour le polling et la détection
void button_update(ButtonContext* context);

// Renvoie et consomme l'événement
ButtonEvent button_getEvent(ButtonContext* context);