#ifndef _BUTTONS_H_
#define _BUTTONS_H_

// Buttons connect between the pin and ground
#define BUTTON_MINUS_PIN 0
#define BUTTON_PLUS_PIN D4
#define BUTTON_ACTION_PIN D2

// Passes button presses to the current mode, repeating minus/plus while held
extern void buttonsSetup();
extern void buttonsLoop();

#endif
