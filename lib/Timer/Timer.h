#ifndef _TIMER_H_
#define _TIMER_H_

#include <Mode.h>

// Countdown timer in 30 second steps. Keeps counting past zero after the alarm.
class TimerMode : public Mode {
public:
  TimerMode();
  const char *name() override { return "Timer"; }
  void open() override;
  void close() override;
  void minus() override;
  void plus() override;
  void action() override;
  void hold() override;
  void update() override;

  void setTime(int seconds);
  void startPause();

private:
  bool active = false;
  unsigned long start = 0;
  int seconds = 0;

  int secondsLeft();
  void reset();
  void display();
};

extern TimerMode timerMode;

#endif
