#include <Arduino.h>
#include "encoder.h"

static int pinA_, pinB_;
static int lastA;

void encoder_init(int pinA, int pinB) {
    pinA_ = pinA;
    pinB_ = pinB;

    pinMode(pinA_, INPUT_PULLUP);
    pinMode(pinB_, INPUT_PULLUP);

    lastA = digitalRead(pinA_);
}

int encoder_getDelta() {
    int A = digitalRead(pinA_);

    if (A != lastA) {
        lastA = A;
        if (A == LOW) {
            return (digitalRead(pinB_) == HIGH) ? +1 : -1;
        }
    }
    return 0;
}
