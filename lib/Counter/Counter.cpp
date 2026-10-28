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
  snprintf(message, BUF_SIZE, "%d", state.counter);
  displayPrint(message);
}

void counterPlus() {
  state.counter++;
  snprintf(message, BUF_SIZE, "%d", state.counter);
  displayPrint(message);
}

void counterAction() {
  state.counter = 0;
  snprintf(message, BUF_SIZE, "%d", state.counter);
  displayPrint(message);
}

bool counterStarted() {
  return state.counter != 0;
}