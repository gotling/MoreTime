#ifndef MORE_TIME_STATE_H
#define MORE_TIME_STATE_H

enum Mode {
  Default = 0,
  ModeTimer = 1,
  TimerPlus = 2,
  StopWatch = 3,
  Counter = 4,
  Menu = 5
};


struct State { 
    Mode mode;
    int counter;
};

extern State state;

#define BUF_SIZE 12
extern char message[BUF_SIZE];

#endif