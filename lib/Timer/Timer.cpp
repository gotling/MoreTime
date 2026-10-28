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
    // Pausing after the alarm stops at zero
    seconds = max(secondsLeft(), 0);
    display();
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
  display();
}

void TimerMode::display() {
  int left = active ? secondsLeft() : seconds;

  if (active && left == 0)
    piezoPlay();

  // Thin font when the text is too long for the wide one
  if (left >= 3600 || left <= -600)
    displayFontThin();
  else
    displayFontWide();

  if (left >= 0)
    displayPrintf("%s", formatDuration(left));
  else
    displayPrintf("+%s", formatDuration(-left));
}
