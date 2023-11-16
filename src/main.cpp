#include <Arduino.h>

#include <Display.h>
#include <Buttons.h>
#include <StopWatch.h>
#include <Network.h>
#include <NTP.h>
#include <MyTime.h>
#include <Menu.h>
#include <Timer.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  displaySetup();
  buttonsSetup();
  networkSetup();
  ntpSetup();

  Serial.println("TiME started");
  timeOpen();
}

void loop() {
  buttonsLoop();
  stopWatchLoop();
  networkLoop();
  ntpLoop();
  timeLoop();
  menuLoop();
  timerLoop();
  displayLoop();
}