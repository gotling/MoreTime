#include <Hum.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Display.h>
#include <Piezo.h>

const char *wifiSleepNames[] = {"none", "light", "modem"};
const char *wifiSleepLabels[] = {"W off", "W lgt", "W mdm"};
const WiFiSleepType_t wifiSleepModes[] = {WIFI_NONE_SLEEP, WIFI_LIGHT_SLEEP, WIFI_MODEM_SLEEP};
#define WIFI_SLEEP_DEFAULT 2

const unsigned int loopDelays[] = {0, 1, 5, 20};
#define LOOP_DELAY_COUNT 4

HumMode humMode;

void HumMode::defaults() {
  brightness = BRIGHTNESS;
  wifiSleep = WIFI_SLEEP_DEFAULT;
  displayOn = true;
  loopDelay = 0;
  piezoLow = false;
}

void HumMode::open() {
  test = Brightness;
  defaults();
  displayFontDefault();
  apply();
  display();
}

// Restore everything to normal operation
void HumMode::close() {
  defaults();
  apply();
  Serial.println("Hum test: closed, defaults restored");
}

void HumMode::minus() {
  change(-1);
}

void HumMode::plus() {
  change(1);
}

void HumMode::action() {
  // Never leave the display off when moving to a test you need to see
  if (test == Display)
    displayOn = true;

  test = static_cast<Test>((test + 1) % (Piezo + 1));
  apply();
  display();
}

void HumMode::change(int direction) {
  switch (test) {
  case Brightness:
    brightness = constrain(brightness + direction, 0, 15);
    break;
  case WiFiSleep:
    wifiSleep = (wifiSleep + direction + 3) % 3;
    break;
  case Display:
    displayOn = !displayOn;
    break;
  case LoopDelay:
    loopDelay = (loopDelay + direction + LOOP_DELAY_COUNT) % LOOP_DELAY_COUNT;
    break;
  case Piezo:
    piezoLow = !piezoLow;
    break;
  }

  apply();
  display();
}

void HumMode::apply() {
  displayBrightness(brightness);
  displayShutdown(!displayOn);
  WiFi.setSleepMode(wifiSleepModes[wifiSleep]);

  if (piezoLow) {
    pinMode(PIEZO_PIN, OUTPUT);
    digitalWrite(PIEZO_PIN, LOW);
  } else {
    piezoIdle();
  }

  Serial.printf("Hum test: brightness=%d wifi_sleep=%s display=%s loop_delay=%ums piezo=%s\n",
    brightness, wifiSleepNames[wifiSleep], displayOn ? "on" : "off",
    loopDelays[loopDelay], piezoLow ? "low" : "high-z");
}

void HumMode::display() {
  switch (test) {
  case Brightness:
    snprintf(message, BUF_SIZE, "Bri %d", brightness);
    break;
  case WiFiSleep:
    snprintf(message, BUF_SIZE, "%s", wifiSleepLabels[wifiSleep]);
    break;
  case Display:
    snprintf(message, BUF_SIZE, "D %s", displayOn ? "on" : "off");
    break;
  case LoopDelay:
    snprintf(message, BUF_SIZE, "L %u", loopDelays[loopDelay]);
    break;
  case Piezo:
    snprintf(message, BUF_SIZE, "P %s", piezoLow ? "low" : "hiZ");
    break;
  }
  displayPrint(message);
}

// Called every loop, to test if a slower loop changes the hum
void HumMode::update() {
  if (loopDelays[loopDelay] > 0)
    delay(loopDelays[loopDelay]);
}
