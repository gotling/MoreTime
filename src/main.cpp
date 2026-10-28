#include <Arduino.h>

#include <Display.h>
#include <Buttons.h>
#include <Network.h>
#include <NTP.h>
#include <Piezo.h>
// #include <_MQTT.h>
#include <Mode.h>
#include <MyTime.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  displaySetup();
  buttonsSetup();
  networkSetup();
  ntpSetup();
  piezoSetup();
  // mqttSetup();

  Serial.println("TiME started");
  modeSwitch(&timeMode);
}

void loop() {
  buttonsLoop();
  modeLoop();
  displayLoop();
  piezoLoop();
  // mqttLoop();
}
