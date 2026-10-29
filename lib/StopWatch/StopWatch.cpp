#include <StopWatch.h>

#include <Arduino.h>

#include <Display.h>
#include <Format.h>
#include <MyTime.h>

StopWatchMode stopWatchMode;

StopWatchMode::StopWatchMode() {
  updateInterval = 100;
}

void StopWatchMode::open() {
  previous = start = 0;
  displayFontWide();
  display();
  startPause();
}

void StopWatchMode::close() {
  running = false;
  previous = start = 0;
}

void StopWatchMode::plus() {
  startPause();
}

void StopWatchMode::action() {
  if (started())
    reset();
  else
    modeSwitch(&timeMode);
}

void StopWatchMode::update() {
  if (running)
    display();
}

void StopWatchMode::startPause() {
  if (!running) {
    running = true;
    start = millis();
    restartUpdates();
  } else {
    running = false;
    previous = previous + (millis() - start);
  }
}

bool StopWatchMode::started() {
  return (start != 0) || (previous != 0);
}

void StopWatchMode::reset() {
  running = false;
  previous = start = 0;
  displayFontWide();
  display();
}

void StopWatchMode::display() {
  if (start == 0) {
    displayPrint("0.0");
    return;
  }

  unsigned long diff = millis() - start + previous;
  unsigned int seconds = diff / 1000;

  if (seconds >= 3600)
    displayFontThin();

  if (seconds >= 60)
    displayPrintf("%s", formatDuration(seconds));
  else
    displayPrintf("%.1f", (float)diff / 1000);
}
