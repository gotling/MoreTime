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

  stopWatchDisplay();
}

void stopWatchClose() {
  stopWatchDisplayTimer.cancel();
  previous = start = 0;
}

void stopWatchMinus() {
}

void stopWatchPlus() {
}

void stopWatchAction() {
  if (stopWatchDisplayTimer.empty()) {
    stopWatchDisplayTimer.every(100, stopWatchDisplayCallback);
    start = millis();
  } else {
    stopWatchDisplayTimer.cancel();
    previous = previous + (millis() - start);
  }
}

void stopWatchLoop() {
  if (state.mode != StopWatch)
    return;
  
  stopWatchDisplayTimer.tick();
}

bool stopWatchStarted() {
  return (start != 0) && (previous != 0);
}

void stopWatchReset() {
  stopWatchDisplayTimer.cancel();
  previous = start = 0;
  stopWatchDisplay();
}

void stopWatchDisplay() {
  if (start == 0) {
    displayPrint((char*)"-.-");
    return;
  }

  unsigned long now = millis();
  unsigned long diff = now - start + previous;
  unsigned int seconds = diff / 1000;

  if (seconds >= 3600) {
    sprintf(message, "%02d:%02d", seconds / 3600, seconds % 3600 - seconds % 60);
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