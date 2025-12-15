#include <Arduino.h>
#include "button.h"

static int btnPin;
static bool lastState = HIGH;

void button_init(int pin) {
    btnPin = pin;
    pinMode(btnPin, INPUT_PULLUP);
}

bool button_wasPressed() {
    bool state = digitalRead(btnPin);

    if (state == LOW && lastState == HIGH) {
        lastState = state;
        return true;
    }

    lastState = state;
    return false;
}
