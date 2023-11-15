#ifndef _MENU_H_
#define _MENU_H_

enum MenuMode {
  MenuMenu = 0,
  MenuStopWatch = 1,
  MenuCounter = 2,
  MenuTime = 3,
  //MenuTimer = 4
};

extern void menuOpen();
extern void menuMinus();
extern void menuPlus();
extern void menuAction();
void displayCurrent();

#endif