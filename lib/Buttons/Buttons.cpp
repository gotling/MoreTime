#include <Buttons.h>
#include <Display.h>
#include <State.h>
#include <Menu.h>
#include <Counter.h>

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

void buttonsSetup() {
  btnList.begin();  // ButtonList calls begin() for each button in the list.
}

void buttonsLoop() {
    btnList.handle();  // ButtonList calls handle() for each button in the list.
}

void minusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Minus");
    minus();
  }
}

void plusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Plus");
    plus();
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
  }
}

void plus() {
  if (state.mode == Counter) {
    counterPlus();
  } else if (state.mode == Menu) {
    menuPlus();
  }
}

void action() {
  if (state.mode == Counter) {
    counterAction();
  } else if (state.mode == Menu) {
    menuAction();
  }
}

void hold() {
  if (state.mode == Counter) {
    menuOpen();
  }
}