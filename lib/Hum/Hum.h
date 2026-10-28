#ifndef _HUM_H_
#define _HUM_H_

// Test mode for finding the source of an audible hum.
// Action cycles through the tests, minus/plus change the value, hold exits.

enum HumTest {
  HumBrightness = 0,
  HumWiFiSleep = 1,
  HumDisplay = 2,
  HumLoopDelay = 3,
  HumPiezo = 4
};

extern void humOpen();
extern void humClose();
extern void humMinus();
extern void humPlus();
extern void humAction();
extern void humLoop();

void humChange(int direction);
void humApply();
void humDisplay();

#endif
