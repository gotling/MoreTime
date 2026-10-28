#include <Hum.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Display.h>
#include <State.h>
#include <Piezo.h>

const char *wifiSleepNames[] = {"none", "light", "modem"};
const char *wifiSleepLabels[] = {"W off", "W lgt", "W mdm"};
const WiFiSleepType_t wifiSleepModes[] = {WIFI_NONE_SLEEP, WIFI_LIGHT_SLEEP, WIFI_MODEM_SLEEP};
#define WIFI_SLEEP_DEFAULT 2

const unsigned int loopDelays[] = {0, 1, 5, 20};
#define LOOP_DELAY_COUNT 4

HumTest humTest = HumBrightness;
int humBrightness = BRIGHTNESS;
int humWiFiSleep = WIFI_SLEEP_DEFAULT;
bool humDisplayOn = true;
int humLoopDelay = 0;
bool humPiezoLow = false;

void humOpen() {
  state.mode = Hum;
  humTest = HumBrightness;
  displayFontDefault();
  humApply();
  humDisplay();
}

// Restore everything to normal operation
void humClose() {
  humBrightness = BRIGHTNESS;
  humWiFiSleep = WIFI_SLEEP_DEFAULT;
  humDisplayOn = true;
  humLoopDelay = 0;
  humPiezoLow = false;
  humApply();
  Serial.println("Hum test: closed, defaults restored");
}

void humMinus() {
  humChange(-1);
}

void humPlus() {
  humChange(1);
}

void humAction() {
  // Never leave the display off when moving to a test you need to see
  if (humTest == HumDisplay)
    humDisplayOn = true;

  humTest = static_cast<HumTest>((humTest + 1) % (HumPiezo + 1));
  humApply();
  humDisplay();
}

void humChange(int direction) {
  switch (humTest) {
  case HumBrightness:
    humBrightness = constrain(humBrightness + direction, 0, 15);
    break;
  case HumWiFiSleep:
    humWiFiSleep = (humWiFiSleep + direction + 3) % 3;
    break;
  case HumDisplay:
    humDisplayOn = !humDisplayOn;
    break;
  case HumLoopDelay:
    humLoopDelay = (humLoopDelay + direction + LOOP_DELAY_COUNT) % LOOP_DELAY_COUNT;
    break;
  case HumPiezo:
    humPiezoLow = !humPiezoLow;
    break;
  }

  humApply();
  humDisplay();
}

void humApply() {
  displayBrightness(humBrightness);
  displayShutdown(!humDisplayOn);
  WiFi.setSleepMode(wifiSleepModes[humWiFiSleep]);

  if (humPiezoLow) {
    pinMode(PIEZO_PIN, OUTPUT);
    digitalWrite(PIEZO_PIN, LOW);
  } else {
    piezoIdle();
  }

  Serial.printf("Hum test: brightness=%d wifi_sleep=%s display=%s loop_delay=%ums piezo=%s\n",
    humBrightness, wifiSleepNames[humWiFiSleep], humDisplayOn ? "on" : "off",
    loopDelays[humLoopDelay], humPiezoLow ? "low" : "high-z");
}

void humDisplay() {
  switch (humTest) {
  case HumBrightness:
    snprintf(message, BUF_SIZE, "Bri %d", humBrightness);
    break;
  case HumWiFiSleep:
    snprintf(message, BUF_SIZE, "%s", wifiSleepLabels[humWiFiSleep]);
    break;
  case HumDisplay:
    snprintf(message, BUF_SIZE, "D %s", humDisplayOn ? "on" : "off");
    break;
  case HumLoopDelay:
    snprintf(message, BUF_SIZE, "L %u", loopDelays[humLoopDelay]);
    break;
  case HumPiezo:
    snprintf(message, BUF_SIZE, "P %s", humPiezoLow ? "low" : "hiZ");
    break;
  }
  displayPrint(message);
}

void humLoop() {
  if (state.mode != Hum)
    return;

  if (loopDelays[humLoopDelay] > 0)
    delay(loopDelays[humLoopDelay]);
}
