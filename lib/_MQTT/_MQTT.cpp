#include <_MQTT.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Timer.h>
#include <Message.h>

WiFiClient net;
MQTTClient client;

void messageReceived(String &topic, String &payload);

void mqttSetup() {
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("10.0.0.3", net);
  client.onMessage(messageReceived);

  connect();
}

void mqttLoop() {
  client.loop();

  if (!client.connected()) {
    delay(10);
    connect();
  }
}

void mqttPublish() {
  client.publish("more-time/timer", "world");
}

void connect() {
  Serial.print("Checking WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConnecting...");
  while (!client.connect("MoreTime", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected!");

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