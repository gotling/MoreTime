#include <Rice.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>

auto riceDisplayTimer = timer_create_default();
bool riceDisplay(void *argument);

unsigned short alternate = 1;

const float step = 0.5;
const short WATER_MULTIPLIER = 180;
const short RICE_MULTIPLIER = 100;

void riceAction();

void riceOpen() {
  state.mode = Rice;
  state.rice = 0;
  alternate = 1;
  displayFontDefault();
  riceStart();
}

void riceClose() {
  riceDisplayTimer.cancel();
  state.rice = 0;
  alternate = 1;
}

void riceLoop() {
  if (state.mode != Rice)
    return;
  
  riceDisplayTimer.tick();
}

void riceMinus() {
  if (state.rice == step)
    return;

  riceDisplayTimer.cancel();
  
  state.rice -= step;
  alternate = 1;
  riceStart();
}

void ricePlus() {
  riceDisplayTimer.cancel();

  state.rice += step;
  alternate = 1;
  riceStart();
}

void riceAction() {}

void riceStart() {
  if (riceDisplayTimer.empty()) {
    riceDisplay();
    riceDisplayTimer.every(1500, riceDisplayCallback);
  } else {
    riceDisplayTimer.cancel();
  }
}

bool riceStarted() {
  return state.rice != 0;
}

void riceSetup() {}

void riceDisplay() {
  if (state.rice == 0) {
    snprintf(message, BUF_SIZE, "Units");
    displayPrint(message);
    return;
  }

  if (alternate == 0) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Units");
    displayPrint(message);
  } else if (alternate == 1) {
    alternate++;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.1f", state.rice);
  } else if (alternate == 2) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Rice");
  } else if (alternate == 3) {
    alternate++;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.0f", state.rice * RICE_MULTIPLIER);
  } else if (alternate == 4) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Water");
  } else if (alternate == 5) {
    alternate = 0;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.0f", state.rice * WATER_MULTIPLIER);
  }

  displayPrint(message);
}

bool riceDisplayCallback(void *argument) {
  riceDisplay();
  return true;
}