#include <Network.h>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include <Display.h>

// Seconds to keep the setup portal open before continuing without WiFi
#define PORTAL_TIMEOUT 180
#define HOSTNAME_SIZE 20

static char hostname[HOSTNAME_SIZE];

const char *networkHostname() {
  return hostname;
}

static void portalStarted(WiFiManager *wm) {
  displayPrint("Setup");
  Serial.println("WiFi setup portal started, connect to " + wm->getConfigPortalSSID());
}

void networkSetup() {
  displayPrint("WiFi");
  snprintf(hostname, HOSTNAME_SIZE, "moretime-%06x", ESP.getChipId());

  WiFiManager wm;
  // Also registered with the router, which often makes it reachable by name
  wm.setHostname(hostname);
  wm.setAPCallback(portalStarted);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  // Connects with saved credentials, or opens a setup portal if there are none or they fail
  String apName = "MoreTime-" + String(ESP.getChipId(), HEX);
  if (wm.autoConnect(apName.c_str())) {
    String ip = WiFi.localIP().toString();
    Serial.printf("WiFi connected, IP: %s, name: %s\n", ip.c_str(), hostname);
    // Show the address for opening the web page on a phone
    displayScroll(ip.c_str());
  } else {
    // The core keeps retrying saved credentials in the background
    Serial.println("WiFi not connected, continuing offline");
  }
}
