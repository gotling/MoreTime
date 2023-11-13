#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

extern void stopWatchOpen();
extern void stopWatchMinus();
extern void stopWatchPlus();
extern void stopWatchAction();
extern void stopWatchLoop();

void stopWatchDisplay();
bool stopWatchTickCallback(void *argument);
bool stopWatchDisplayCallback(void *argument);

#endif