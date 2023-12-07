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
    timer.every(250, timer_minus);
  } else if (event == Button::RELEASED_EVENT) {
    timer.cancel();
  }
}

void plusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Plus");
    plus();
    timer.every(250, timer_plus);
  } else if (event == Button::RELEASED_EVENT) {
    timer.cancel();
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
    piezoPlay();
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
  }
}

bool timer_minus(void *argument) {
    minus();
    return true;
}

bool timer_plus(void *argument) {
    plus();
    return true;
}