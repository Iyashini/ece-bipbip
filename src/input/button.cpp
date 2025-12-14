// include/input/button.h
#pragma once

enum ButtonEvent {
    BUTTON_NONE = 0,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS
};

void button_init(int pin);

// À appeler à chaque loop()
void button_update();

// Renvoie l’événement détecté depuis le dernier appel (ou BUTTON_NONE)
ButtonEvent button_getEvent();
