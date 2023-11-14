#ifndef _MENU_H_
#define _MENU_H_

enum MenuMode {
  MenuMenu = 0,
  //MenuTimer = 1,
  MenuStopWatch = 2,
  MenuCounter = 3,
  MenuTime = 4
};

extern void menuOpen();
extern void menuMinus();
extern void menuPlus();
extern void menuAction();
void displayCurrent();

#endif