#include <Mode.h>

#include <Arduino.h>

#include <MyTime.h>

Mode *current = nullptr;

void Mode::hold() {
  modeSwitch(&timeMode);
}

void Mode::restartUpdates() {
  lastUpdate = millis();
}

void modeSwitch(Mode *mode) {
  if (current)
    current->close();

  Serial.print("Mode: ");
  Serial.println(mode->name());

  current = mode;
  current->restartUpdates();
  current->open();
}

Mode *modeCurrent() {
  return current;
}

void modeLoop() {
  if (!current)
    return;

  if (millis() - current->lastUpdate >= current->updateInterval) {
    current->lastUpdate = millis();
    current->update();
  }
}
