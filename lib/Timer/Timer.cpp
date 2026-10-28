#include <Timer.h>

#include <Arduino.h>

#include <Display.h>
#include <Piezo.h>
#include <MyTime.h>

#define STEP_SECONDS 30

TimerMode timerMode;

TimerMode::TimerMode() {
  updateInterval = 500;
}

void TimerMode::open() {
  start = seconds = 0;
  active = false;
  displayFontWide();
  display();
}

void TimerMode::close() {
  seconds = 0;
  active = false;
}

void TimerMode::minus() {
  if (active)
    return;

  seconds -= STEP_SECONDS;

  if (seconds < 0)
    seconds = 0;
  display();
}

void TimerMode::plus() {
  if (active)
    return;

  seconds += STEP_SECONDS;
  display();
}

void TimerMode::action() {
  if (seconds == 0)
    modeSwitch(&timeMode);
  else
    startPause();
}

void TimerMode::hold() {
  if (!active && seconds == 0)
    modeSwitch(&timeMode);
  else
    reset();
}

void TimerMode::update() {
  if (active)
    display();
}

void TimerMode::setTime(int newSeconds) {
  seconds = newSeconds;
}

void TimerMode::startPause() {
  if (!active) {
    active = true;
    start = millis();
    restartUpdates();
  } else {
    active = false;
    seconds = secondsLeft();
  }
}

int TimerMode::secondsLeft() {
  unsigned long diffMs = millis() - start;
  unsigned int diff = diffMs / 1000;
  return seconds - diff;
}

void TimerMode::reset() {
  seconds = 0;
  active = false;
  displayFontWide();
  display();
}

void TimerMode::display() {
  if (!active) {
    snprintf(message, BUF_SIZE, "%d:%02d", seconds / 60, seconds % 60);
    displayPrint(message);
    return;
  }

  int left = secondsLeft();

  if (left == 0) {
    piezoPlay();
  }

  if (left >= 0) {
    snprintf(message, BUF_SIZE, "%d:%02d", left / 60, left % 60);
  } else if (left <= -3600) {
    displayFontThin();
    snprintf(message, BUF_SIZE, "+%d:%02d:%02d", -1 * left / 3600, -1 * (left % 3600) / 60, -1 * left % 60);
  } else if (left <= -600) {
    displayFontThin();
    snprintf(message, BUF_SIZE, "+%02d:%02d", -1 * (left % 3600) / 60, -1 * left % 60);
  } else {
    snprintf(message, BUF_SIZE, "+%d:%02d", -1 * left / 60, -1 * left % 60);
  }

  displayPrint(message);
}
