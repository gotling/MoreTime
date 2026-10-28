#include <Counter.h>

#include <Display.h>
#include <MyTime.h>

CounterMode counterMode;

void CounterMode::open() {
  displayFontWide();
  reset();
}

void CounterMode::minus() {
  counter--;
  display();
}

void CounterMode::plus() {
  counter++;
  display();
}

void CounterMode::action() {
  if (counter != 0)
    reset();
  else
    modeSwitch(&timeMode);
}

void CounterMode::reset() {
  counter = 0;
  display();
}

void CounterMode::display() {
  snprintf(message, BUF_SIZE, "%d", counter);
  displayPrint(message);
}
