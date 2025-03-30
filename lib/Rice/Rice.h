#ifndef _RICE_H_
#define _RICE_H_

extern void riceOpen();
extern void riceMinus();
extern void ricePlus();
extern void riceAction();
extern void riceStart();
extern bool riceStarted();
extern void riceSetup();
extern void riceLoop();
extern void riceClose();

void riceDisplay();
bool riceDisplayCallback(void *argument);

#endif