#include <StopWatch.h>

#include <Arduino.h>

#include <Display.h>
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
    snprintf(message, BUF_SIZE, "0.0");
    displayPrint(message);
    return;
  }

  unsigned long diff = millis() - start + previous;
  unsigned int seconds = diff / 1000;

  if (seconds >= 3600) {
    displayFontThin();
    snprintf(message, BUF_SIZE, "%d:%02d:%02d", seconds / 3600, (seconds % 3600) / 60, seconds % 60);
  } else if (seconds >= 600) {
    snprintf(message, BUF_SIZE, "%02d:%02d", seconds / 60, seconds % 60);
  } else if (seconds >= 60) {
    snprintf(message, BUF_SIZE, "%d:%02d", seconds / 60, seconds % 60);
  } else {
    snprintf(message, BUF_SIZE, "%.1f", (float)diff / 1000);
  }

  displayPrint(message);
}
