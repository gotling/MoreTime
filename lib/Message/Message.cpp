#include <Message.h>

#include <arduino-timer.h>
#include <ArduinoJson.h>

#include <Arduino.h>

#include <Display.h>
#include <State.h>
#include <MyTime.h>

auto messageTimer = timer_create_default();

unsigned long messageStart = 0;

String messages[4] = {"0", "1", "2", "3"};
unsigned short messageIndex = 0;
unsigned short messageDisplayIndex = 0;
StaticJsonDocument<200> doc;

void messageAdd(String &message) {
  deserializeJson(doc, message);

  short index = doc["position"];
  if ((index < 0) || (index > 3))
    return;
  
  messages[index] = doc["value"].as<String>();

  if ((state.mode == Message) && (index == messageDisplayIndex))
    messageDisplay();
}

void messageSetup() {
}

void messageLoop() {
  if (state.mode != Message)
    return;
  
  messageTimer.tick();
}

void messageOpen() {
  state.mode = Message;
  messageStart = 0;
  messageDisplayIndex = 1;
  messageDisplay();
}

void messageClose() {
}

void messageDisplay() {
  displayFontDefault();
  
  displayPrint((char *)messages[messageDisplayIndex].c_str());
}

void messageMinus() {
  if (messageDisplayIndex == 0)
    messageDisplayIndex = 3;
  else
    messageDisplayIndex--;
  
  messageDisplay();
}

void messagePlus() {
  if (messageDisplayIndex >= 3)
    messageDisplayIndex = 0;
  else
    messageDisplayIndex++;
  
  messageDisplay();
}

void messageAction() {

}

