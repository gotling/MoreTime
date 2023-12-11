#ifndef _CHESS_H_
#define _CHESS_H_

extern void chessOpen();
extern void chessClose();

extern void chessMinus();
extern void chessPlus();
extern bool chessAction();
extern bool chessHold();

extern void chessSetup();
extern void chessLoop();
extern void chessStart();
extern bool chessStarted();
extern void chessReset();

void chessDisplay();
bool chessDisplayCallback(void *argument);
int secondsLeftWhite();
int secondsLeftBlack();

#endif