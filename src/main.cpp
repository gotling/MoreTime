#include <Arduino.h>

#include <Display.h>
#include <Buttons.h>

enum mode {
  Default = 0,
  Timer = 1,
  TimerPlus = 2,
  StopWatch = 3
};

// GLOBALS
mode state = Default;
// END GLOBALS

void setup() {
  Serial.begin(115200);

  displaySetup();
  buttonsSetup();

  Serial.println("TiME started");
}

void loop() {
  buttonsLoop();
}