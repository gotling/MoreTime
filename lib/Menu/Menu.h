#ifndef _MENU_H_
#define _MENU_H_

enum MenuMode {
  MenuStopWatch = 0,
  MenuCounter = 1,
  MenuTime = 2,
  //MenuTimer = 4
};

extern void menuOpen();
extern void menuMinus();
extern void menuPlus();
extern void menuAction();
void displayCurrent();
void resetIdleTimer();
bool menuIdleCallback(void *argument);
void menuLoop();

#endif