#include <Buttons.h>

#include <Display.h>

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

// Display
#define BUF_SIZE 8
char message[BUF_SIZE];
int counter = 0;

void buttonsSetup() {
  btnList.begin();  // ButtonList calls begin() for each button in the list.
}

void buttonsLoop() {
    btnList.handle();  // ButtonList calls handle() for each button in the list.
}

void minusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Minus");
    counter--;
    sprintf(message, "%d", counter);
    displayPrint(message);
  }
}

void plusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Plus");
    counter++;
    sprintf(message, "%d", counter);
    displayPrint(message);
  }
}

void actionCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Action");
    counter = 0;
    sprintf(message, "%d", counter);
    displayPrint(message);
  }
}