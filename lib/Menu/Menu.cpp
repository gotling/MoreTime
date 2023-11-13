#include <Menu.h>
#include <Display.h>
#include <State.h>
#include <Counter.h>

MenuMode menuMode = MenuMenu;

void menuOpen() {
  state.mode = Menu;
  menuMode = MenuMenu;
  displayCurrent();
}

void menuMinus() {
  if (menuMode == MenuMenu)
    menuMode = MenuCounter;
  else
    menuMode = static_cast<MenuMode>((menuMode - 1));
  Serial.print("MenuMinus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuPlus() {
  menuMode = static_cast<MenuMode>((menuMode + 1) % (MenuCounter + 1));
  Serial.print("MenuPlus: ");
  Serial.println(menuMode);
  displayCurrent();
}

void menuAction() {
  Serial.print("MenuAction: ");
  Serial.println(menuMode);

  if (menuMode == MenuCounter)
    counterOpen();
}

void displayCurrent() {
  switch (menuMode)
  {
  case MenuMenu:
    displayPrint((char*)"Menu");
    break;
  case MenuTimer:
    displayPrint((char*)"Timer");
    break;
  case MenuStopWatch:
    displayPrint((char*)"Stop Watch");
    break;
  case MenuCounter:
    displayPrint((char*)"Counter");
    break;
  default:
    break;
  }
}