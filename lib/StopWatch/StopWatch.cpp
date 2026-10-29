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
  show(start == 0 ? 0 : millis() - start + previous);
}

void StopWatchMode::show(unsigned long elapsedMs) {
  unsigned int seconds = elapsedMs / 1000;

  if (seconds >= 3600)
    displayFontThin();
  else
    displayFontWide();

  if (seconds >= 60)
    displayPrintf("%s", formatDuration(seconds));
  else
    displayPrintf("%.1f", (float)elapsedMs / 1000);
}

RemoteState StopWatchMode::remoteState() {
  RemoteState state;
  if (!started())
    return state;

  state.kind = RemoteState::StopWatch;
  state.running = running;
  state.ms = previous;
  if (running)
    state.ms += millis() - start;
  return state;
}
