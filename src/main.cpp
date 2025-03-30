#include <Arduino.h>

#include <Display.h>
#include <Buttons.h>
#include <StopWatch.h>
#include <Network.h>
#include <NTP.h>
#include <MyTime.h>
#include <Menu.h>
#include <Timer.h>
#include <Piezo.h>
#include <_MQTT.h>
#include <Message.h>
#include <Chess.h>
#include <Rice.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  displaySetup();
  buttonsSetup();
  networkSetup();
  ntpSetup();
  piezoSetup();
  mqttSetup();
  chessSetup();
  riceSetup();

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
  piezoLoop();
  mqttLoop();
  messageLoop();
  chessLoop();
  riceLoop();
}