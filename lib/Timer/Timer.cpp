#include <Timer.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Piezo.h>

auto timerDisplayTimer = timer_create_default();

unsigned static long start = 0;
int timerSeconds = 0;
bool active = false;

bool timerDisplay(void *argument);

void timerOpen() {
  state.mode = MyTimer;
  start = timerSeconds = 0;
  active = false;
  displayFontWide();
  timerDisplay();
}

void timerClose() {
  timerDisplayTimer.cancel();
  timerSeconds = 0;
  active = false;
}

void timerMinus() {
  if (active)
    return;
  
  timerSeconds -= 30;

  if (timerSeconds < 0)
    timerSeconds = 0;
  timerDisplay();
}

void timerPlus() {
  if (active)
    return;
  
  timerSeconds += 30;
  timerDisplay();
}

bool timerAction() {
  if (timerSeconds == 0)
    return false;
  
  timerStart();
  return true;
}

bool timerHold() {
  if (!active && timerSeconds == 0)
    return false;
  
  timerReset();
  return true;
}

void timerLoop() {
  if (state.mode != MyTimer)
    return;
  
  timerDisplayTimer.tick();
}

void timerStart() {
  if (timerDisplayTimer.empty()) {
    active = true;
    timerDisplayTimer.every(500, timerDisplayCallback);
    start = millis();
  } else {
    active = false;
    timerSeconds = secondsLeft();
    timerDisplayTimer.cancel();
  }
}

bool timerStarted() {
  return start != 0;
}

void timerReset() {
  timerDisplayTimer.cancel();
  timerSeconds = 0;
  active = false;
  displayFontWide();
  timerDisplay();
}

int secondsLeft() {
  unsigned long diffMs = millis() - start;
  unsigned int diff = diffMs / 1000;
  return timerSeconds - diff;
}

void timerSetTime(unsigned int seconds) {
  timerSeconds = seconds;
}

void timerDisplay() {
  if (!active) {
    sprintf(message, "%d:%02d", timerSeconds / 60, timerSeconds % 60);
    displayPrint(message);
    return;
  }

  int seconds = secondsLeft();

  if (seconds == 0) {
    piezoPlay();
  }

  if (seconds >= 0) {
    sprintf(message, "%d:%02d", seconds / 60, seconds % 60);
  } else if (seconds <= -3600) {
    displayFontThin();
    sprintf(message, "+%d:%02d:%02d", -1 * seconds / 3600, -1 * (seconds % 3600) / 60, -1 * seconds % 60);
  } else if (seconds <= -600) {
    displayFontThin();
    sprintf(message, "+%02d:%02d", -1 * (seconds % 3600) / 60, -1 * seconds % 60);
  } else {
    sprintf(message, "+%d:%02d", -1 * seconds / 60, -1 * seconds % 60);
  }

  displayPrint(message);
}

bool timerDisplayCallback(void *argument) {
  timerDisplay();
  return true;
}