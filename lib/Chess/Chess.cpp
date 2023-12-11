#include <Chess.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Piezo.h>

auto chessDisplayTimer = timer_create_default();

unsigned static long startWhite = 0;
unsigned static long startBlack = 0;
int chessSeconds;
int chessSecondsWhite;
int chessSecondsBlack;
bool chessActive = false;
bool chessWhite = true;

bool chessDisplay(void *argument);
void chessSwitch(boolean white);

void chessOpen() {
  state.mode = Chess;
  startWhite = startBlack = 0;
  chessSeconds = chessSecondsWhite = chessSecondsBlack = 15 * 60;
  chessActive = false;
  displayFontThin();
  chessDisplay();
}

void chessClose() {
  chessDisplayTimer.cancel();
  chessSeconds = chessSecondsWhite = chessSecondsBlack = 0;
  chessActive = false;
}

void chessMinus() {
  if (chessActive) {
    // Switch to black
    chessSwitch(false);
    return;
  }
  
  chessSeconds -= 300;

  if (chessSeconds < 300)
    chessSeconds = 300;
  chessDisplay();
}

void chessPlus() {
  if (chessActive)
    // Nothing or pause
    return;
  
  chessSeconds += 300;
  chessDisplay();
}

bool chessAction() {
  if (chessActive) {
    // Switch to white
    chessSwitch(true);
    //return false;
  }

  if (chessSeconds == 0)
    return false;
  
  chessStart();
  return true;
}

bool chessHold() {
  if (!chessActive && chessSeconds == 0)
    return false;
  
  chessReset();
  return true;
}

void chessSetup() {}

void chessLoop() {
  if (state.mode != Chess)
    return;
  
  chessDisplayTimer.tick();
}

void chessSwitch(boolean white) {
  if (white) {
    chessWhite = true;
    chessSecondsBlack = secondsLeftBlack();
    chessSecondsBlack += 10;
    startWhite = millis();
  } else {
    chessWhite = false;
    chessSecondsWhite = secondsLeftWhite();
    chessSecondsWhite += 10;
    startBlack = millis();
  }
  chessDisplay();
}

void chessStart() {
  if (chessDisplayTimer.empty()) {
    Serial.println("Start Chess");
    chessSecondsWhite = chessSecondsBlack = chessSeconds;
    chessActive = true;
    chessDisplayTimer.every(500, chessDisplayCallback);
    startWhite = startBlack = millis();
  }
}

bool chessStarted() {
  return chessSeconds != 0;
}

void chessReset() {
  chessDisplayTimer.cancel();
  chessSeconds = chessSecondsWhite = chessSecondsBlack = 0;
  chessActive = false;
  displayFontThin();
  chessDisplay();
}

int secondsLeftWhite() {
  unsigned long diffMs = millis() - startWhite;
  unsigned int diff = diffMs / 1000;
  return chessSecondsWhite - diff;
}

int secondsLeftBlack() {
  unsigned long diffMs = millis() - startBlack;
  unsigned int diff = diffMs / 1000;
  return chessSecondsBlack - diff;
}

void chessDisplay() {
  if (!chessActive) {
    sprintf(message, "%d:%02d", chessSeconds / 60, chessSeconds % 60);
    displayPrint(message);
    return;
  }

  int secondsBlack = secondsLeftBlack();
  int secondsWhite = secondsLeftWhite();

  if ((secondsWhite <= 0) || (secondsBlack <= 0)) {
    piezoPlay();

    if (secondsWhite <= 0)
      sprintf(message, "Black");
    else
      sprintf(message, "White");

    chessDisplayTimer.cancel();
    displayFontDefault();
  } else {
    if (chessWhite)
      sprintf(message, "< %d:%02d", secondsWhite / 60, secondsWhite % 60);
    else
      sprintf(message, "%d:%02d >", secondsBlack / 60, secondsBlack % 60);
  }

  displayPrint(message);
}

bool chessDisplayCallback(void *argument) {
  chessDisplay();
  return true;
}