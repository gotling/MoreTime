#include <Web.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Network.h>
#include <Control.h>
#include <Remote.h>

#include "Page.h"
#include "Icons.h"

#define HTTP_PORT 80
#define JSON_SIZE 192

static ESP8266WebServer server(HTTP_PORT);

static void sendState(int code) {
  char json[JSON_SIZE];
  remoteEncode(controlState(), networkHostname(), json, JSON_SIZE);
  server.send(code, "application/json", json);
}

static void sendError(int code, const char *message) {
  char json[JSON_SIZE];
  snprintf(json, JSON_SIZE, "{\"error\":\"%s\"}", message);
  server.send(code, "application/json", json);
}

static void handlePage() {
  server.send_P(200, "text/html", PAGE);
}

static void handleManifest() {
  server.send_P(200, "application/manifest+json", MANIFEST);
}

static void handleServiceWorker() {
  // Let the browser pick up a new version after a firmware update
  server.sendHeader("Cache-Control", "no-cache");
  server.send_P(200, "text/javascript", SERVICE_WORKER);
}

static void sendIcon(const uint8_t *icon, size_t size) {
  server.sendHeader("Cache-Control", "max-age=86400");
  server.send_P(200, "image/png", (PGM_P)icon, size);
}

static void handleRequest() {
  String uri = server.uri();

  if (uri == "/api/state") {
    sendState(200);
    return;
  }

  if (!uri.startsWith("/api/")) {
    sendError(404, "Not found");
    return;
  }

  if (server.method() != HTTP_POST) {
    sendError(405, "Use POST");
    return;
  }

  long seconds = server.hasArg("seconds") ? server.arg("seconds").toInt() : -1;
  String command = uri.substring(5);

  switch (controlRun(command.c_str(), seconds)) {
  case ControlOk:
    Serial.printf("Web: %s\n", command.c_str());
    sendState(200);
    break;
  case ControlUnknown:
    sendError(404, "Unknown command");
    break;
  case ControlInvalid:
    sendError(400, "Seconds missing or out of range");
    break;
  case ControlInactive:
    sendError(409, "Not running on the device");
    break;
  }
}

void webSetup() {
  server.on("/", HTTP_GET, handlePage);
  server.on("/manifest.json", HTTP_GET, handleManifest);
  server.on("/sw.js", HTTP_GET, handleServiceWorker);
  server.on("/icon-192.png", HTTP_GET, []() { sendIcon(ICON_192, sizeof(ICON_192)); });
  server.on("/icon-512.png", HTTP_GET, []() { sendIcon(ICON_512, sizeof(ICON_512)); });
  server.onNotFound(handleRequest);
  server.begin();

  if (MDNS.begin(networkHostname()))
    MDNS.addService("http", "tcp", HTTP_PORT);

  Serial.printf("Web: http://%s.local\n", networkHostname());
}

void webLoop() {
  server.handleClient();
  MDNS.update();
}
