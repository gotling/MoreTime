#include <Menu.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Counter.h>
#include <StopWatch.h>
#include <MyTime.h>
#include <Timer.h>
#include <Chess.h>

MenuMode menuMode = Undefind;

auto menuIdleTimer = timer_create_default();

void menuOpen() {
  state.mode = Menu;
  if ((menuMode == Undefind) || (menuMode == MenuTime))
    menuMode = MenuStopWatch;
  
  displayFontDefault();
  displayCurrent();

  resetIdleTimer();
}

void resetIdleTimer() {
  menuIdleTimer.cancel();
  menuIdleTimer.in(10000, menuIdleCallback);
}

void menuMinus() {
  resetIdleTimer();
  if (menuMode == MenuStopWatch)
    menuMode = MenuChess;
  else
    menuMode = static_cast<MenuMode>((menuMode - 1));
  Serial.print("MenuMinus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuPlus() {
  resetIdleTimer();
  menuMode = static_cast<MenuMode>((menuMode + 1) % (MenuChess + 1));
  Serial.print("MenuPlus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuAction() {
  Serial.print("MenuAction: ");
  Serial.println(menuMode);

  if (menuMode == MenuCounter)
    counterOpen();
  else if (menuMode == MenuStopWatch)
    stopWatchOpen();
  else if (menuMode == MenuTimer)
    timerOpen();
  else if (menuMode == MenuTime)
    timeOpen();
  else if (menuMode == MenuChess)
    chessOpen();
}

void displayCurrent() {
  switch (menuMode)
  {
  case MenuStopWatch:
    sprintf(message, "Chrono");
    break;
  case MenuTimer:
    sprintf(message, "Timer");
    break;
  case MenuCounter:
    sprintf(message, "Counter");
    break;
  case MenuTime:
    sprintf(message, "Time");
    break;
  case MenuChess:
    sprintf(message, "Chess");
    break;
  default:
    sprintf(message, "Unknown");
    break;
  }
  displayPrint(message);
}

bool menuIdleCallback(void *argument) {
  timeOpen();
  return true;
}

void menuLoop() {
  if (state.mode != Menu)
    return;
  
  menuIdleTimer.tick();
}