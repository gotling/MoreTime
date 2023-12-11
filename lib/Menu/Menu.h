#ifndef _MENU_H_
#define _MENU_H_

enum MenuMode {
  Undefind = -1,
  MenuStopWatch = 0,
  MenuTimer = 1,
  MenuCounter = 2,
  MenuTime = 3,
  MenuChess = 4
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