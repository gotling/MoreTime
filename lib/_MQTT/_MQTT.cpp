#include <_MQTT.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Timer.h>
#include <Message.h>

WiFiClient net;
MQTTClient client;

// Retry in the background so a missing broker doesn't block buttons and display
#define RECONNECT_INTERVAL 30000
// Keep each attempt short, it blocks the main loop while running
#define CONNECT_TIMEOUT 1000

unsigned long lastConnectAttempt = 0;

void messageReceived(String &topic, String &payload);

void mqttSetup() {
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  net.setTimeout(CONNECT_TIMEOUT);
  client.setTimeout(CONNECT_TIMEOUT);
  client.begin("10.0.0.3", net);
  client.onMessage(messageReceived);

  connect();
}

void mqttLoop() {
  client.loop();

  if (!client.connected() && millis() - lastConnectAttempt > RECONNECT_INTERVAL)
    connect();
}

void mqttPublish() {
  client.publish("more-time/timer", "world");
}

void connect() {
  lastConnectAttempt = millis();

  if (WiFi.status() != WL_CONNECTED)
    return;

  Serial.print("MQTT connecting...");
  if (!client.connect("MoreTime", "public", "public")) {
    Serial.printf(" failed (error %d, return code %d)\n", client.lastError(), client.returnCode());
    return;
  }

  Serial.println(" connected!");

  client.subscribe("more-time/#");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming: " + topic + " - " + payload);

  if (topic == "more-time/message") {
    Serial.println("Message");
    messageAdd(payload);
  } else if (topic == "more-time/timer") {
    Serial.println("Timer");
    timerOpen();
    timerSetTime(atoi(payload.c_str()));
    timerStart();
  }
}