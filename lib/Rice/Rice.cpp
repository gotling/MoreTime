#include <Rice.h>

#include <Display.h>
#include <MyTime.h>

const float step = 0.5;
const short WATER_MULTIPLIER = 180;
const short RICE_MULTIPLIER = 100;

RiceMode riceMode;

RiceMode::RiceMode() {
  updateInterval = 1500;
}

void RiceMode::open() {
  rice = 0;
  alternate = 1;
  displayFontDefault();
  update();
}

void RiceMode::close() {
  rice = 0;
  alternate = 1;
}

void RiceMode::minus() {
  if (rice <= step)
    return;

  change(-step);
}

void RiceMode::plus() {
  change(step);
}

void RiceMode::action() {
  if (rice == 0)
    modeSwitch(&timeMode);
}

// Show the new amount right away and keep it up for a full interval
void RiceMode::change(float amount) {
  rice += amount;
  alternate = 1;
  update();
  restartUpdates();
}

// Cycles through label and value for units, rice and water
void RiceMode::update() {
  if (rice == 0) {
    displayPrint("Units");
    return;
  }

  switch (alternate) {
  case 0:
    displayFontDefault();
    displayPrint("Units");
    break;
  case 1:
    displayFontWide();
    displayPrintf("%.1f", rice);
    break;
  case 2:
    displayFontDefault();
    displayPrint("Rice");
    break;
  case 3:
    displayFontWide();
    displayPrintf("%.0f", rice * RICE_MULTIPLIER);
    break;
  case 4:
    displayFontDefault();
    displayPrint("Water");
    break;
  case 5:
    displayFontWide();
    displayPrintf("%.0f", rice * WATER_MULTIPLIER);
    break;
  }

  alternate = (alternate + 1) % 6;
}
