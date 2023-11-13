#include <StopWatch.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>

auto stopWatchDisplayTimer = timer_create_default();
auto stopWatchTickTimer = timer_create_default();

int stopWatchSeconds = 0;

bool stopWatchDisplay(void *argument);
bool stopWatchTick(void *argument);

void stopWatchOpen() {
  state.mode = StopWatch;
  stopWatchSeconds = 0;

  stopWatchDisplay();
}

void stopWatchMinus() {
}

void stopWatchPlus() {
}

void stopWatchAction() {
  if (stopWatchTickTimer.empty()) {
    stopWatchDisplayTimer.every(500, stopWatchDisplayCallback);
    stopWatchTickTimer.every(1000, stopWatchTickCallback);
  } else {
    stopWatchDisplayTimer.cancel();
    stopWatchTickTimer.cancel();
  }
}

void stopWatchLoop() {
  if (state.mode != StopWatch)
    return;
  
  stopWatchDisplayTimer.tick();
  stopWatchTickTimer.tick();
}

void stopWatchDisplay() {
  if (stopWatchSeconds >= 3600) {
    sprintf(message, "%02d:%02d", stopWatchSeconds / 3600, stopWatchSeconds % 3600 - stopWatchSeconds % 60);
  } else if (stopWatchSeconds >= 600) {
    sprintf(message, "%02d:%02d", stopWatchSeconds / 60, stopWatchSeconds % 60);
  } else if (stopWatchSeconds >= 60) {
    sprintf(message, "%d:%02d", stopWatchSeconds / 60, stopWatchSeconds % 60);
  } else {
    sprintf(message, "%d", stopWatchSeconds);
  }

  displayPrint(message);
}

bool stopWatchTickCallback(void *argument) {
  stopWatchSeconds++;
  return true;
}

bool stopWatchDisplayCallback(void *argument) {
  stopWatchDisplay();
  return true;
}