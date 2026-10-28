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

void RiceMode::update() {
  if (rice == 0) {
    snprintf(message, BUF_SIZE, "Units");
    displayPrint(message);
    return;
  }

  if (alternate == 0) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Units");
    displayPrint(message);
  } else if (alternate == 1) {
    alternate++;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.1f", rice);
  } else if (alternate == 2) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Rice");
  } else if (alternate == 3) {
    alternate++;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.0f", rice * RICE_MULTIPLIER);
  } else if (alternate == 4) {
    alternate++;
    displayFontDefault();
    snprintf(message, BUF_SIZE, "Water");
  } else if (alternate == 5) {
    alternate = 0;
    displayFontWide();
    snprintf(message, BUF_SIZE, "%.0f", rice * WATER_MULTIPLIER);
  }

  displayPrint(message);
}
