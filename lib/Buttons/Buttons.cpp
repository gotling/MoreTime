#include <Buttons.h>

#include <AbleButtons.h>
#include <arduino-timer.h>

#include <Mode.h>

using Button = AblePullupCallbackButton;
using ButtonList = AblePullupCallbackButtonList;

// Delay before minus/plus start repeating, repeats speed up the longer they are held
#define REPEAT_MS 250

static void minusCallback(Button::CALLBACK_EVENT event, uint8_t id);
static void plusCallback(Button::CALLBACK_EVENT event, uint8_t id);
static void actionCallback(Button::CALLBACK_EVENT event, uint8_t id);

static Button btnMinus(BUTTON_MINUS_PIN, minusCallback);
static Button btnPlus(BUTTON_PLUS_PIN, plusCallback);
static Button btnAction(BUTTON_ACTION_PIN, actionCallback);

static Button *btns[] = {
  &btnMinus,
  &btnPlus,
  &btnAction
};
static ButtonList btnList(btns);

static auto repeatTimer = timer_create_default();
static short repeatMs = REPEAT_MS;
static short repeatCount = 0;

void buttonsSetup() {
  btnList.begin();
}

void buttonsLoop() {
  btnList.handle();
  repeatTimer.tick();
}

static void resetRepeat() {
  repeatMs = REPEAT_MS;
  repeatCount = 0;
  repeatTimer.cancel();
}

static void updateRepeat(bool (*operation)(void *argument)) {
  repeatCount++;
  repeatTimer.cancel();

  if (repeatCount > 20)
    repeatMs = 25;
  else if (repeatCount > 10)
    repeatMs = 50;
  else if (repeatCount > 5)
    repeatMs = 100;
  else if (repeatCount > 2)
    repeatMs = 200;

  repeatTimer.every(repeatMs, operation);
}

static bool repeatMinus(void *argument) {
  modeCurrent()->minus();
  updateRepeat(repeatMinus);
  return true;
}

static bool repeatPlus(void *argument) {
  modeCurrent()->plus();
  updateRepeat(repeatPlus);
  return true;
}

static void minusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Minus");
    modeCurrent()->minus();
    repeatTimer.every(repeatMs, repeatMinus);
  } else if (event == Button::RELEASED_EVENT) {
    resetRepeat();
  }
}

static void plusCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Plus");
    modeCurrent()->plus();
    repeatTimer.every(repeatMs, repeatPlus);
  } else if (event == Button::RELEASED_EVENT) {
    resetRepeat();
  }
}

static void actionCallback(Button::CALLBACK_EVENT event, uint8_t id) {
  if (event == Button::PRESSED_EVENT) {
    Serial.println("Action");
    modeCurrent()->action();
  } else if (event == Button::HELD_EVENT) {
    Serial.println("Hold");
    modeCurrent()->hold();
  }
}
