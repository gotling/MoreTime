#include <Message.h>

#include <ArduinoJson.h>

#include <Display.h>
#include <MyTime.h>

MessageMode messageMode;

StaticJsonDocument<200> doc;

void MessageMode::add(String &json) {
  deserializeJson(doc, json);

  short index = doc["position"];
  if ((index < 0) || (index >= MESSAGE_COUNT))
    return;

  messages[index] = doc["value"].as<String>();

  if ((modeCurrent() == this) && (index == displayIndex))
    display();
}

void MessageMode::open() {
  displayIndex = 1;
  display();
}

void MessageMode::minus() {
  if (displayIndex == 0)
    displayIndex = MESSAGE_COUNT - 1;
  else
    displayIndex--;

  display();
}

void MessageMode::plus() {
  if (displayIndex >= MESSAGE_COUNT - 1)
    displayIndex = 0;
  else
    displayIndex++;

  display();
}

void MessageMode::action() {
  modeSwitch(&timeMode);
}

void MessageMode::display() {
  displayFontDefault();

  displayPrint((char *)messages[displayIndex].c_str());
}
