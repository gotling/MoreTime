#include <Sync.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Remote.h>
#include <Control.h>
#include <Mirror.h>

#define SYNC_PORT 4210
// Send at least this often while something is active, the mirror times out after 5 s
#define HEARTBEAT_MS 1000
// Repeat the stop message in case a packet is lost
#define STOP_REPEATS 3
#define PACKET_SIZE 192
#define ID_SIZE 9

static WiFiUDP udp;
static char deviceId[ID_SIZE];

static RemoteState lastSent;
static unsigned long lastSentAt = 0;
static int stopRepeats = 0;

static RemoteState lastReceived;

void syncSetup() {
  snprintf(deviceId, ID_SIZE, "%06x", ESP.getChipId());
  udp.begin(SYNC_PORT);
  Serial.printf("Sync: device %s listening on UDP %d\n", deviceId, SYNC_PORT);
}

static void send(const RemoteState &state) {
  char packet[PACKET_SIZE];
  if (!remoteEncode(state, deviceId, packet, PACKET_SIZE))
    return;

  udp.beginPacket(WiFi.broadcastIP(), SYNC_PORT);
  udp.write(packet);
  udp.endPacket();

  if (remoteChanged(state, lastSent))
    Serial.printf("Sync: sent %s\n", packet);

  lastSent = state;
  lastSentAt = millis();
}

static void sendLocal() {
  RemoteState state = controlState();
  bool changed = remoteChanged(state, lastSent);
  bool due = millis() - lastSentAt >= HEARTBEAT_MS;

  if (state.kind != RemoteState::None) {
    if (changed || due)
      send(state);
    stopRepeats = STOP_REPEATS;
  } else if (stopRepeats > 0 && (changed || due)) {
    send(state);
    stopRepeats--;
  }
}

static void receive() {
  int size = udp.parsePacket();
  if (size <= 0)
    return;

  char packet[PACKET_SIZE];
  int length = udp.read(packet, PACKET_SIZE - 1);
  if (length <= 0)
    return;
  packet[length] = '\0';

  RemoteState state;
  char sender[ID_SIZE];
  if (!remoteDecode(packet, state, sender, ID_SIZE))
    return;

  // Broadcasts can come back to the sender
  if (strcmp(sender, deviceId) == 0)
    return;

  if (remoteChanged(state, lastReceived))
    Serial.printf("Sync: received %s\n", packet);
  lastReceived = state;

  mirrorMode.receive(state);
}

void syncLoop() {
  if (WiFi.status() != WL_CONNECTED)
    return;

  receive();
  sendLocal();
}
