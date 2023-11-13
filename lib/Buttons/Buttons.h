#include <AbleButtons.h>

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

// Identify which buttons you are using...
using Button = AblePullupCallbackButton;          ///< Using clicker pull-up button.
using ButtonList = AblePullupCallbackButtonList;  ///< Using clicker pull-up button list.

#define BUTTON_A_PIN 0   // Connect button A between this pin and ground.
#define BUTTON_B_PIN D4  // Connect button B between this pin and ground.
#define BUTTON_C_PIN D2  // Connect button B between this pin and ground.

// Declaration of callback function defined later.
extern void buttonsSetup();
extern void buttonsLoop();
void minusCallback(Button::CALLBACK_EVENT event, uint8_t id);
void plusCallback(Button::CALLBACK_EVENT event, uint8_t id);
void actionCallback(Button::CALLBACK_EVENT event, uint8_t id);
void plus();
void minus();
void action();
void hold();
bool timer_minus(void *argument);
bool timer_plus(void *argument);

#endif