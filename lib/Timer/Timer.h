#ifndef _TIMER_H_
#define _TIMER_H_

extern void timerOpen();
extern void timerClose();

extern void timerMinus();
extern void timerPlus();
extern bool timerAction();
extern bool timerHold();

extern void timerLoop();
extern void timerStart();
extern bool timerStarted();
extern void timerReset();
extern void timerSetTime(unsigned int seconds);

int secondsLeft();
void timerDisplay();
bool timerDisplayCallback(void *argument);

#endif