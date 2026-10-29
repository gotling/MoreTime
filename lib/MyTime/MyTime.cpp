#include <MyTime.h>

#include <Display.h>
#include <NTP.h>
#include <Menu.h>
#include <Rice.h>
#include <StopWatch.h>

TimeMode timeMode;

TimeMode::TimeMode() {
  updateInterval = 1000;
}

void TimeMode::open() {
  displayFontWide();
  update();
}

// Shortcuts from the clock
void TimeMode::minus() {
  modeSwitch(&riceMode);
}

void TimeMode::plus() {
  modeSwitch(&stopWatchMode);
}

void TimeMode::action() {
  modeSwitch(&menuMode);
}

void TimeMode::hold() {
  modeSwitch(&menuMode);
}

void TimeMode::update() {
  // Placeholder until the time is synced, so the display is never blank
  if (ntpTimeSet())
    displayPrintf("%02d:%02d", ntpHour(), ntpMinute());
  else
    displayPrint("--:--");
}
