#ifndef _CHESS_H_
#define _CHESS_H_

#include <Mode.h>

// Chess clock. Before start minus/plus set the time in 5 minute steps.
// During a game minus hands the turn to black and action to white.
class ChessMode : public Mode {
public:
  ChessMode();
  const char *name() override { return "Chess"; }
  void open() override;
  void close() override;
  void minus() override;
  void plus() override;
  void action() override;
  void hold() override;
  void update() override;

private:
  bool active = false;
  bool ticking = false;
  bool over = false;
  bool white = true;
  unsigned long startWhite = 0;
  unsigned long startBlack = 0;
  int seconds = 0;
  int secondsWhite = 0;
  int secondsBlack = 0;

  void switchTo(bool toWhite);
  void start();
  void reset();
  int secondsLeftWhite();
  int secondsLeftBlack();
  void display();
};

extern ChessMode chessMode;

#endif
