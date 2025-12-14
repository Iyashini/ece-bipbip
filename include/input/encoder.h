#pragma once
#include "button.h" // Nécessaire pour le type ButtonEvent

void encoder_init(int pinA, int pinB, int pinSW);

// Appelée dans loop() pour le polling du bouton de l'encodeur
void encoder_update();

// Renvoie le delta de rotation (-1, 0, +1)
int encoder_getDelta();

// Renvoie l'événement du bouton de l'encodeur (via module button)
ButtonEvent encoder_getSwitchEvent();