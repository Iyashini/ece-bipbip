#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "input/encoder.h"
#include "input/button.h"
#include "display/menu.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ENCODEUR
#define ENC_A 3
#define ENC_B 4
#define ENC_BTN 2

enum State {
    STATE_MENU,
    STATE_COMPOSE,
    STATE_SENT
};

State currentState = STATE_MENU;

// TABLE DE CARACTÈRES
const char charset[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?.,- ";
const int charsetSize = sizeof(charset) - 1;

char messageBuffer[32] = {0};
int charIndex = 0;
int messageIndex = 0;

void setup() {
    Serial.begin(9600);

    // Désactiver buzzer
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);

    // OLED FIRST !!!
    Wire.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Erreur OLED");
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("OLED OK (Main)");
    display.display();
    delay(500);

    encoder_init(ENC_A, ENC_B);
    button_init(ENC_BTN);

    menu_init();
}

void loop() {

    int delta = encoder_getDelta();
    bool clicked = button_wasPressed();

    switch (currentState) {
    
    case STATE_MENU:
        menu_update(delta, clicked);
        if (clicked) {
            int c = menu_getSelected();
            if (c == 0) {
                messageIndex = 0;
                messageBuffer[0] = '\0';
                charIndex = 0;
                currentState = STATE_COMPOSE;
            }
        }
        break;

    case STATE_COMPOSE:
        charIndex = (charIndex + delta + charsetSize) % charsetSize;

        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Composer message:");

        display.setTextSize(2);
        display.setCursor(0, 15);
        display.print("[ ");
        display.print(charset[charIndex]);
        display.print(" ]");

        display.setTextSize(1);
        display.setCursor(0, 50);
        display.print("Msg: ");
        display.println(messageBuffer);

        display.display();

        if (clicked) {
            if (messageIndex < 31) {
                messageBuffer[messageIndex++] = charset[charIndex];
                messageBuffer[messageIndex] = '\0';
            }
        }
        break;

    case STATE_SENT:
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Message envoye !");
        display.println("");
        display.println("Cliquez pour retour");
        display.display();

        if (clicked)
            currentState = STATE_MENU;

        break;
    }
}
