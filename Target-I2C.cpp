#include <Wire.h>

// Updated LED pins: D5, D7, D9, D11, D13
const int ledPins[] = {5, 7, 9, 11, 13};
const int numLeds = 5;

int ledState = 0;                 // Tracks which LED is on
bool sequenceActive = false;     // Whether sequence is running
unsigned long ledTimer = 0;
const unsigned long interval = 500;

void setup() {
  Wire.begin(8);                 // Join I2C bus as slave with address 8
  Wire.onReceive(receiveEvent); // Register receive event

  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  if (sequenceActive && ledState > 0 && ledState < numLeds) {
    if (millis() - ledTimer >= interval) {
      digitalWrite(ledPins[ledState], HIGH);
      ledState++;
      ledTimer = millis();
    }
  }
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char command = Wire.read();

    if (command == 'S' && !sequenceActive) {
      sequenceActive = true;
      ledState = 1;
      digitalWrite(ledPins[0], HIGH);
      ledTimer = millis();
    }
    else if (command == 'R' && ledState == numLeds) {
      // Only reset if all LEDs were on
      sequenceActive = false;
      ledState = 0;
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}
