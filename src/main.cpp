#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "display/animation.h"
#include "display/menu.h"

// =========================
//   Définition des pins
// =========================

// Encodeur rotatif
const int PIN_ENC_A  = 3;
const int PIN_ENC_B  = 4;
const int PIN_ENC_SW = 2;

// Bouton poussoir
const int PIN_BUTTON = A6;

// LED RGB
const int PIN_LED_R = 5;
const int PIN_LED_G = 6;
const int PIN_LED_B = 9;

// LED verte
const int PIN_LED_VERTE = A0;

// Buzzer
const int PIN_BUZZER = 10;

// NRF24
const int PIN_RF24_CE  = 7;
const int PIN_RF24_CSN = 8;

// =============================
//   Écran OLED SSD1306
// =============================
Adafruit_SSD1306 display(128, 64, &Wire);

// État interface
int lastEncA        = HIGH;
int currentScreen   = 0;    // 0 = menu principal
bool clickLocked    = false;
bool buttonWasPressed = false;

// =============================
//   Écran : ENVOYER MESSAGE
// =============================
void screenEnvoyerMessage() {
    display.clearDisplay();

    display.drawRect(0, 0, 128, 64, WHITE);

    display.setCursor(20, 5);
    display.setTextColor(WHITE);
    display.print("Envoyer un msg");

    display.setCursor(10, 25);
    display.print("Appuie pour ecrire");

    display.setCursor(10, 45);
    display.print("(clic = retour)");

    display.display();
}

// =============================
//   SETUP GLOBAL
// =============================
void setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_LED_VERTE, OUTPUT);

    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_RF24_CE, OUTPUT);
    pinMode(PIN_RF24_CSN, OUTPUT);
    pinMode(10, OUTPUT);    // contrainte SPI / buzzer

    animationDemarrage(display);
    delay(300);
    afficherMenu(display);
}
