#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

extern void stopWatchOpen();
extern void stopWatchClose();
extern void stopWatchMinus();
extern void stopWatchPlus();
extern void stopWatchAction();
extern void stopWatchLoop();
extern void stopWatchStart();
extern bool stopWatchStarted();
extern void stopWatchReset();

void stopWatchDisplay();
bool stopWatchDisplayCallback(void *argument);

#endif