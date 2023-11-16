#include <MyTime.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <NTP.h>

auto timeDisplayTimer = timer_create_default();

void timeOpen() {
  state.mode = Time;
  displayFontWide();
  timeDisplay();
  timeDisplayTimer.every(1000, timeDisplayCallback);
}

void timeClose() {
    timeDisplayTimer.cancel();
}

void timeMinus() {

}

void timePlus() {

}

void timeAction() {

}

void timeDisplay() {
  if (ntpTimeSet()) {
    sprintf(message, "%02d:%02d", ntpHour(), ntpMinute());
    displayPrint(message);
  }
}

bool timeDisplayCallback(void *argument) {
  timeDisplay();
  return true;
}

void timeLoop() {
  if (state.mode != Time)
    return;
  
  timeDisplayTimer.tick();
}