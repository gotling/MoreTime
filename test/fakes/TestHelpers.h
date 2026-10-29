#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <Arduino.h>
#include <Display.h>
#include <Piezo.h>
#include <Mode.h>

// Start each test from a clean slate with the given mode open. Time starts
// at 1 s since the device has always been running a while when a mode opens.
inline void resetTo(Mode *mode) {
  fakeMillis = 1000;
  piezoPlays = 0;
  displayText = "";
  displayFont = FontDefault;
  modeSwitch(mode);
}

// Move time forward in 10 ms steps, running the main loop like the device does
inline void advance(unsigned long ms) {
  for (unsigned long passed = 0; passed < ms; passed += 10) {
    fakeMillis += 10;
    modeLoop();
  }
}

#endif
