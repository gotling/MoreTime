#include <Network.h>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include <Display.h>

// Seconds to keep the setup portal open before continuing without WiFi
#define PORTAL_TIMEOUT 180

static void portalStarted(WiFiManager *wm) {
  displayPrint("Setup");
  Serial.println("WiFi setup portal started, connect to " + wm->getConfigPortalSSID());
}

void networkSetup() {
  displayPrint("WiFi");

  WiFiManager wm;
  wm.setAPCallback(portalStarted);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  // Connects with saved credentials, or opens a setup portal if there are none or they fail
  String apName = "MoreTime-" + String(ESP.getChipId(), HEX);
  if (wm.autoConnect(apName.c_str())) {
    Serial.print("WiFi connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    // The core keeps retrying saved credentials in the background
    Serial.println("WiFi not connected, continuing offline");
  }
}
