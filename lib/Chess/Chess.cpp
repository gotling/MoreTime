#include <Chess.h>

#include <Arduino.h>

#include <Display.h>
#include <Piezo.h>
#include <MyTime.h>

#define DEFAULT_SECONDS (15 * 60)
#define STEP_SECONDS 300
#define INCREMENT_SECONDS 10

ChessMode chessMode;

ChessMode::ChessMode() {
  updateInterval = 500;
}

void ChessMode::open() {
  startWhite = startBlack = 0;
  seconds = secondsWhite = secondsBlack = DEFAULT_SECONDS;
  active = false;
  ticking = false;
  over = false;
  displayFontThin();
  display();
}

void ChessMode::close() {
  ticking = false;
  over = false;
  seconds = secondsWhite = secondsBlack = 0;
  active = false;
}

void ChessMode::minus() {
  if (over)
    return;

  if (active) {
    switchTo(false);
    return;
  }

  seconds -= STEP_SECONDS;

  if (seconds < STEP_SECONDS)
    seconds = STEP_SECONDS;
  display();
}

void ChessMode::plus() {
  if (active)
    return;

  seconds += STEP_SECONDS;
  display();
}

void ChessMode::action() {
  // Back to the setup screen with the same time, ready for a new game
  if (over) {
    over = false;
    active = false;
    displayFontThin();
    display();
    return;
  }

  if (active)
    switchTo(true);

  if (seconds == 0)
    modeSwitch(&timeMode);
  else
    start();
}

void ChessMode::hold() {
  if (!active && seconds == 0)
    modeSwitch(&timeMode);
  else
    reset();
}

void ChessMode::update() {
  if (ticking)
    display();
}

void ChessMode::switchTo(bool toWhite) {
  if (toWhite) {
    white = true;
    secondsBlack = secondsLeftBlack() + INCREMENT_SECONDS;
    startWhite = millis();
  } else {
    white = false;
    secondsWhite = secondsLeftWhite() + INCREMENT_SECONDS;
    startBlack = millis();
  }
  display();
}

void ChessMode::start() {
  if (ticking)
    return;

  Serial.println("Start Chess");
  secondsWhite = secondsBlack = seconds;
  active = true;
  ticking = true;
  restartUpdates();
  startWhite = startBlack = millis();
}

void ChessMode::reset() {
  ticking = false;
  over = false;
  seconds = secondsWhite = secondsBlack = 0;
  active = false;
  displayFontThin();
  display();
}

int ChessMode::secondsLeftWhite() {
  unsigned long diffMs = millis() - startWhite;
  unsigned int diff = diffMs / 1000;
  return secondsWhite - diff;
}

int ChessMode::secondsLeftBlack() {
  unsigned long diffMs = millis() - startBlack;
  unsigned int diff = diffMs / 1000;
  return secondsBlack - diff;
}

void ChessMode::display() {
  if (!active) {
    displayPrintf("%s", formatMinutes(seconds));
    return;
  }

  int leftBlack = secondsLeftBlack();
  int leftWhite = secondsLeftWhite();

  if ((leftWhite <= 0) || (leftBlack <= 0)) {
    piezoPlay();
    ticking = false;
    over = true;
    // Show the winner
    displayFontDefault();
    displayPrint(leftWhite <= 0 ? "Black" : "White");
  } else if (white) {
    displayPrintf("< %s", formatMinutes(leftWhite));
  } else {
    displayPrintf("%s >", formatMinutes(leftBlack));
  }
}
