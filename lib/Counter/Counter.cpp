#include <Counter.h>
#include <Display.h>
#include <State.h>

void counterAction();

void counterOpen() {
  state.mode = Counter;
  displayFontWide();
  counterAction();
}

void counterMinus() {
  state.counter--;
  sprintf(message, "%d", state.counter);
  displayPrint(message);
}

void counterPlus() {
  state.counter++;
  sprintf(message, "%d", state.counter);
  displayPrint(message);
}

void counterAction() {
  state.counter = 0;
  sprintf(message, "%d", state.counter);
  displayPrint(message);
}

bool counterStarted() {
  return state.counter != 0;
}