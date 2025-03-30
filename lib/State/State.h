#ifndef MORE_TIME_STATE_H
#define MORE_TIME_STATE_H

enum Mode {
  Default = 0,
  MyTimer = 1,
  TimerPlus = 2,
  StopWatch = 3,
  Counter = 4,
  Menu = 5,
  Time = 6,
  Message = 7,
  Chess = 8,
  Rice = 9
};

struct State { 
    Mode mode;
    int counter;
    int rice;
};

extern State state;

#endif