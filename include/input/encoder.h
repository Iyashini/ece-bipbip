#pragma once
#include "button.h" // Nécessaire pour l'énumération ButtonEvent

void encoder_init(int pinA, int pinB, int pinSW);

// À appeler à chaque loop() (nécessaire pour le polling du bouton de l'encodeur)
void encoder_update();

// Rotation : -1, 0, +1
int encoder_getDelta();

// Événements de clic complexe (via le module button)
ButtonEvent encoder_getSwitchEvent();