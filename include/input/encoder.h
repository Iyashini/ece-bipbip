#pragma once

void encoder_init(int pinA, int pinB, int pinSW);

// À appeler à chaque loop() - Requis uniquement pour la détection de clic (anti-rebond logiciel)
void encoder_update();

// -1, 0, +1 selon la rotation depuis le dernier appel
int encoder_getDelta();

// true une seule fois quand un clic court vient d’être détecté
bool encoder_wasClicked();