#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <Mode.h>

// Count up and down. Action resets, or returns to the clock when already zero.
class CounterMode : public Mode {
public:
  const char *name() override { return "Counter"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;

private:
  int counter = 0;

  void reset();
  void display();
};

extern CounterMode counterMode;

#endif
