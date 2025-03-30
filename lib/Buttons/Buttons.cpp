#include <Buttons.h>

#include <arduino-timer.h>

#include <Display.h>
#include <State.h>
#include <Menu.h>
#include <Counter.h>
#include <StopWatch.h>
#include <MyTime.h>
#include <Timer.h>
#include <Piezo.h>
#include <Message.h>
#include <Chess.h>
#include <Rice.h>

#define REPEAT_MS 250
short repeat_ms = REPEAT_MS;
short repeat_count = 0;

Button btnA(BUTTON_A_PIN, minusCallback);   // Primary button.
Button btnB(BUTTON_B_PIN, plusCallback);    // Secondary button.
Button btnC(BUTTON_C_PIN, actionCallback);  // Third button.

// Array of buttons for ButtonList.
Button *btns[] = {
  &btnA,
  &btnB,
  &btnC
};
ButtonList btnList(btns);  // List of button to control together.

auto timer = timer_create_default();

void buttonsSetup() {
  btnList.begin();  // ButtonList calls begin() for each button in the list.
}

void buttonsLoop() {
    btnList.handle();  // ButtonList calls handle() for each button in the list.
    timer.tick();
}

void minusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Minus");
    minus();
    timer.every(repeat_ms, timer_minus);
  } else if (event == Button::RELEASED_EVENT) {
    reset_repeat();
  }
}

void plusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Plus");
    plus();
    timer.every(repeat_ms, timer_plus);
  } else if (event == Button::RELEASED_EVENT) {
    reset_repeat();
  }
}

void actionCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Action");
    action();
  } else if (event == Button::HELD_EVENT) {
    Serial.println("Hold");
    hold();
  }
}

void minus() {
  if (state.mode == Counter) {
    counterMinus();
  } else if (state.mode == Menu) {
    menuMinus();
  } else if (state.mode == MyTimer) {
    timerMinus();
  } else if (state.mode == Time) {
    messageOpen();
  } else if (state.mode == Message) {
    messageMinus();
  } else if (state.mode == Chess) {
    chessMinus();
  } else if (state.mode == Rice) {
    riceMinus();
  }
}

void plus() {
  if (state.mode == Counter) {
    counterPlus();
  } else if (state.mode == Menu) {
    menuPlus();
  } else if (state.mode == StopWatch) {
    stopWatchPlus();
  } else if (state.mode == MyTimer) {
    timerPlus();
  } else if (state.mode == Time) {
    stopWatchOpen();
  } else if (state.mode == Message) {
    messagePlus();
  } else if (state.mode == Chess) {
    chessPlus();
  } else if (state.mode == Rice) {
    ricePlus();
  }
}

void action() {
  if (state.mode == Counter) {
    if (counterStarted())
      counterAction();
    else
      timeOpen();
  } else if (state.mode == Menu) {
    menuAction();
  } else if (state.mode == StopWatch) {
    if (stopWatchStarted())
      stopWatchAction();
    else
      timeOpen();
  } else if (state.mode == Time) {
    menuOpen();
  } else if (state.mode == MyTimer) {
    if (!timerAction())
      timeOpen();
  } else if (state.mode == Message) {
    timeOpen();
  } else if (state.mode == Chess) {
    if (!chessAction())
      timeOpen();
  } else if (state.mode == Rice) {
    if (riceStarted())
      riceAction();
    else {
      riceClose();
      timeOpen();
    }
  }
}

void hold() {
  if (state.mode == Counter) {
    timeOpen();
  } else if (state.mode == StopWatch) {
    stopWatchClose();
    timeOpen();
  } else if (state.mode == Time) {
    menuOpen();
  } else if (state.mode == MyTimer) {
    if (!timerHold())
      timeOpen();
  } else if (state.mode == Chess) {
    if (!chessHold())
      timeOpen();
  } else if (state.mode == Rice) {
    riceClose();
    timeOpen();
  } 
}

void reset_repeat() {
  repeat_ms = REPEAT_MS;
  repeat_count = 0;
  timer.cancel();
}

void update_repeat(bool (*operation)(void *argument)) {
  repeat_count++;
  timer.cancel();

  if (repeat_count > 20)
    repeat_ms = 25;
  else if (repeat_count > 10)
    repeat_ms = 50;
  else if (repeat_count > 5)
    repeat_ms = 100;
  else if (repeat_count > 2)
    repeat_ms = 200;

  timer.every(repeat_ms, *operation);
}

bool timer_minus(void *argument) {
    minus();
    update_repeat(timer_minus);
    return true;
}

bool timer_plus(void *argument) {
    plus();
    update_repeat(timer_plus);
    return true;
}