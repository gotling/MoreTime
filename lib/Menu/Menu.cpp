#include <Menu.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Counter.h>
#include <StopWatch.h>
#include <MyTime.h>

MenuMode menuMode = MenuStopWatch;

auto menuIdleTimer = timer_create_default();

void menuOpen() {
  state.mode = Menu;
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
    menuMode = MenuTime;
  else
    menuMode = static_cast<MenuMode>((menuMode - 1));
  Serial.print("MenuMinus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuPlus() {
  resetIdleTimer();
  menuMode = static_cast<MenuMode>((menuMode + 1) % (MenuTime + 1));
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
  else if (menuMode == MenuTime)
    timeOpen();
}

void displayCurrent() {
  switch (menuMode)
  {
  // case MenuMenu:
  //   displayPrint((char*)"Menu");
  //   break;
  // case MenuTimer:
  //   displayPrint((char*)"Timer");
  //   break;
  case MenuStopWatch:
    displayPrint((char*)"Stop Watch");
    break;
  case MenuCounter:
    displayPrint((char*)"Counter");
    break;
  case MenuTime:
    displayPrint((char*)"Time");
    break;
  default:
    break;
  }
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