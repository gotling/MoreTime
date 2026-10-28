#include <Menu.h>

#include <Arduino.h>

#include <Display.h>
#include <MyTime.h>
#include <StopWatch.h>
#include <Timer.h>
#include <Counter.h>
#include <Chess.h>
#include <Rice.h>
#include <Sound.h>
#include <Hum.h>

#define IDLE_MS 10000

// Add new modes here to show them in the menu
Mode *menuItems[] = {
  &stopWatchMode,
  &timerMode,
  &counterMode,
  &timeMode,
  &chessMode,
  &riceMode,
  &soundMode,
  &humMode,
};
const int menuItemCount = sizeof(menuItems) / sizeof(Mode *);

MenuMode menuMode;

MenuMode::MenuMode() {
  updateInterval = IDLE_MS;
}

void MenuMode::open() {
  // Start from the top unless the last choice is worth coming back to
  if (menuItems[index] == &timeMode)
    index = 0;

  displayFontDefault();
  display();
}

void MenuMode::minus() {
  restartUpdates();
  index = (index + menuItemCount - 1) % menuItemCount;
  display();
}

void MenuMode::plus() {
  restartUpdates();
  index = (index + 1) % menuItemCount;
  display();
}

void MenuMode::action() {
  modeSwitch(menuItems[index]);
}

// No button pressed for IDLE_MS
void MenuMode::update() {
  modeSwitch(&timeMode);
}

void MenuMode::display() {
  displayPrint(menuItems[index]->name());
}
