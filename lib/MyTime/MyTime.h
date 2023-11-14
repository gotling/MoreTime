#ifndef _MYTIME_H_
#define _MYTIME_H_

extern void timeOpen();
extern void timeClose();
extern void timeMinus();
extern void timePlus();
extern void timeAction();
extern void timeLoop();

void timeDisplay();
bool timeDisplayCallback(void *argument);

#endif