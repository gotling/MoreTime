#include <Ip.h>

#include <ESP8266WiFi.h>

#include <Display.h>
#include <MyTime.h>

IpMode ipMode;

void IpMode::open() {
  displayFontDefault();
  scroll();
}

void IpMode::minus() {
  modeSwitch(&timeMode);
}

void IpMode::plus() {
  modeSwitch(&timeMode);
}

void IpMode::action() {
  modeSwitch(&timeMode);
}

// Start over each time the address has scrolled past
void IpMode::update() {
  if (displayScrollDone())
    scroll();
}

void IpMode::scroll() {
  if (WiFi.status() == WL_CONNECTED)
    displayScrollStart(WiFi.localIP().toString().c_str());
  else
    displayScrollStart("No WiFi");
}
