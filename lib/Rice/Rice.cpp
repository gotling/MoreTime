#include <Rice.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>

auto riceDisplayTimer = timer_create_default();
bool riceDisplay(void *argument);

unsigned short alternate = 1;

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
  if (state.rice == 1)
    return;
  
  state.rice--;
  alternate = 1;
  displayFontWide();
  sprintf(message, "%d", state.rice);
  displayPrint(message);
}

void ricePlus() {
  state.rice++;
  alternate = 1;
  displayFontWide();
  sprintf(message, "%d", state.rice);
  displayPrint(message);
}

void riceAction() {}

void riceStart() {
  if (riceDisplayTimer.empty()) {
    riceDisplay();
    riceDisplayTimer.every(2000, riceDisplayCallback);
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
    sprintf(message, "Units");
    displayPrint(message);
    return;
  }

  if (alternate == 0) {
    alternate++;
    displayFontDefault();
    sprintf(message, "Units");
    displayPrint(message);
  } else if (alternate == 1) {
    alternate++;
    displayFontWide();
    sprintf(message, "%d", state.rice);
  } else if (alternate == 2) {
    alternate++;
    displayFontDefault();
    sprintf(message, "Rice");
  } else if (alternate == 3) {
    alternate++;
    displayFontWide();
    sprintf(message, "%d", state.rice * RICE_MULTIPLIER);
  } else if (alternate == 4) {
    alternate++;
    displayFontDefault();
    sprintf(message, "Water");
  } else if (alternate == 5) {
    alternate = 0;
    displayFontWide();
    sprintf(message, "%d", state.rice * WATER_MULTIPLIER);
  }

  displayPrint(message);
}

bool riceDisplayCallback(void *argument) {
  riceDisplay();
  return true;
}