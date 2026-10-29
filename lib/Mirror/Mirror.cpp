#include <Mirror.h>

#include <Arduino.h>

#include <Display.h>
#include <Piezo.h>
#include <MyTime.h>
#include <Timer.h>
#include <StopWatch.h>
#include <Chess.h>

// Other devices send at least every second while active
#define TIMEOUT_MS 5000

MirrorMode mirrorMode;

MirrorMode::MirrorMode() {
  updateInterval = 100;
}

void MirrorMode::receive(const RemoteState &received) {
  bool wasOver = state.over;
  bool newTimer = received.kind == RemoteState::Timer && state.kind != RemoteState::Timer;

  state = received;
  receivedAt = millis();

  if (newTimer)
    timerLeft = state.ms > 0;

  if (state.kind == RemoteState::None) {
    dismissed = RemoteState::None;
    if (modeCurrent() == this)
      modeSwitch(&timeMode);
    return;
  }

  if (modeCurrent() == this) {
    if (state.kind == RemoteState::Chess && state.over && !wasOver)
      piezoPlay();
    display();
  } else if (modeCurrent() == &timeMode && state.kind != dismissed) {
    modeSwitch(this);
  }
}

void MirrorMode::open() {
  display();
}

void MirrorMode::minus() {
  dismiss();
}

void MirrorMode::plus() {
  dismiss();
}

void MirrorMode::action() {
  dismiss();
}

void MirrorMode::hold() {
  dismiss();
}

void MirrorMode::update() {
  if (millis() - receivedAt > TIMEOUT_MS) {
    modeSwitch(&timeMode);
    return;
  }

  display();
}

void MirrorMode::dismiss() {
  dismissed = state.kind;
  modeSwitch(&timeMode);
}

// Time since the last message, the mirror keeps counting on its own in between
long MirrorMode::elapsed() {
  return state.running ? (long)(millis() - receivedAt) : 0;
}

void MirrorMode::display() {
  switch (state.kind) {
  case RemoteState::Timer: {
    int left = remoteSeconds(state.ms - elapsed());
    if (timerLeft && left <= 0)
      piezoPlay();
    timerLeft = left > 0;
    TimerMode::show(left);
    break;
  }
  case RemoteState::StopWatch:
    StopWatchMode::show(state.ms + elapsed());
    break;
  case RemoteState::Chess:
    if (state.over) {
      ChessMode::showWinner(state.white);
    } else {
      long left = (state.white ? state.ms : state.blackMs) - elapsed();
      ChessMode::show(max(remoteSeconds(left), 0), state.white);
    }
    break;
  case RemoteState::None:
    break;
  }
}
