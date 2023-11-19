#ifndef MORE_TIME_STATE_H
#define MORE_TIME_STATE_H

enum Mode {
  Default = 0,
  MyTimer = 1,
  TimerPlus = 2,
  StopWatch = 3,
  Counter = 4,
  Menu = 5,
  Time = 6
};

struct State { 
    Mode mode;
    int counter;
};

extern State state;

#endif