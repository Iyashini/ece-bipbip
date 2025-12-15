#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include "radio.h"

#define PIN_CE 7
#define PIN_CSN 8

RF24 radio(PIN_CE, PIN_CSN);

const byte address[6] = "ABC01";

void radio_init() {
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);
    radio.openWritingPipe(address);
    radio.stopListening();
}

void radio_sendMessage(const char* msg) {
    radio.write(msg, strlen(msg) + 1);
    Serial.print("Radio envoye : ");
    Serial.println(msg);
}
