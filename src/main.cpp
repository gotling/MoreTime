#include <Arduino.h>

#include <Display.h>
#include <Buttons.h>

void setup() {
  Serial.begin(115200);

  displaySetup();
  buttonsSetup();

  Serial.println("TiME started");
}

void loop() {
  buttonsLoop();
}