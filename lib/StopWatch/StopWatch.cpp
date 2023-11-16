#include <StopWatch.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>

auto stopWatchDisplayTimer = timer_create_default();

unsigned long start = 0;
unsigned long previous = 0;

bool stopWatchDisplay(void *argument);

void stopWatchOpen() {
  state.mode = StopWatch;
  previous = start = 0;
  displayFontWide();
  stopWatchDisplay();
  stopWatchStart();
}

void stopWatchClose() {
  stopWatchDisplayTimer.cancel();
  previous = start = 0;
}

void stopWatchMinus() {
}

void stopWatchPlus() {
  stopWatchStart();
}

void stopWatchAction() {
  stopWatchReset();
}

void stopWatchLoop() {
  if (state.mode != StopWatch)
    return;
  
  stopWatchDisplayTimer.tick();
}

void stopWatchStart() {
  if (stopWatchDisplayTimer.empty()) {
    stopWatchDisplayTimer.every(100, stopWatchDisplayCallback);
    start = millis();
  } else {
    stopWatchDisplayTimer.cancel();
    previous = previous + (millis() - start);
  }
}

bool stopWatchStarted() {
  return (start != 0) || (previous != 0);
}

void stopWatchReset() {
  stopWatchDisplayTimer.cancel();
  previous = start = 0;
  displayFontWide();
  stopWatchDisplay();
}

void stopWatchDisplay() {
  if (start == 0) {
    sprintf(message, "0.0");
    displayPrint(message);
    return;
  }

  unsigned long diff = millis() - start + previous;
  unsigned int seconds = diff / 1000;

  if (seconds >= 3600) {
    displayFontThin();
    sprintf(message, "%d:%02d:%02d", seconds / 3600, (seconds % 3600) / 60, seconds % 60);
  } else if (seconds >= 600) {
    sprintf(message, "%02d:%02d", seconds / 60, seconds % 60);
  } else if (seconds >= 60) {
    sprintf(message, "%d:%02d", seconds / 60, seconds % 60);
  } else {
    sprintf(message, "%.1f", (float)diff / 1000);
  }

  displayPrint(message);
}

bool stopWatchDisplayCallback(void *argument) {
  stopWatchDisplay();
  return true;
}