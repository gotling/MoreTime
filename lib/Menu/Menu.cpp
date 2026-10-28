#include <Menu.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Counter.h>
#include <StopWatch.h>
#include <MyTime.h>
#include <Timer.h>
#include <Chess.h>
#include <Rice.h>
#include <Hum.h>

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
    menuMode = MenuHum;
  else
    menuMode = static_cast<MenuMode>((menuMode - 1));
  Serial.print("MenuMinus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuPlus() {
  resetIdleTimer();
  menuMode = static_cast<MenuMode>((menuMode + 1) % (MenuHum + 1));
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
  else if (menuMode == MenuRice)
    riceOpen();
  else if (menuMode == MenuHum)
    humOpen();
}

void displayCurrent() {
  switch (menuMode)
  {
  case MenuStopWatch:
    snprintf(message, BUF_SIZE, "Chrono");
    break;
  case MenuTimer:
    snprintf(message, BUF_SIZE, "Timer");
    break;
  case MenuCounter:
    snprintf(message, BUF_SIZE, "Counter");
    break;
  case MenuTime:
    snprintf(message, BUF_SIZE, "Time");
    break;
  case MenuChess:
    snprintf(message, BUF_SIZE, "Chess");
    break;
  case MenuRice:
    snprintf(message, BUF_SIZE, "Rice");
    break;
  case MenuHum:
    snprintf(message, BUF_SIZE, "Hum");
    break;
  default:
    snprintf(message, BUF_SIZE, "Unknown");
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