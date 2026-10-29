#include <Remote.h>

#include <string.h>

#include <ArduinoJson.h>

static const char *kindNames[] = {"none", "timer", "stopwatch", "chess"};

// Fits all fields of a chess message plus the strings copied when parsing
#define DOCUMENT_SIZE (JSON_OBJECT_SIZE(8) + 64)
#define KIND_COUNT 4

bool remoteEncode(const RemoteState &state, const char *id, char *buffer, size_t size) {
  StaticJsonDocument<DOCUMENT_SIZE> doc;
  doc["id"] = id;
  doc["mode"] = kindNames[state.kind];

  if (state.kind != RemoteState::None) {
    doc["running"] = state.running;
    doc["ms"] = state.ms;
  }

  if (state.kind == RemoteState::Chess) {
    doc["black"] = state.blackMs;
    doc["white"] = state.white;
    doc["over"] = state.over;
  }

  size_t length = serializeJson(doc, buffer, size);
  return length > 0 && length < size;
}

bool remoteDecode(const char *json, RemoteState &state, char *id, size_t idSize) {
  StaticJsonDocument<DOCUMENT_SIZE> doc;
  if (deserializeJson(doc, json))
    return false;

  const char *sender = doc["id"];
  const char *mode = doc["mode"];
  if (!sender || !mode)
    return false;

  int kind = -1;
  for (int i = 0; i < KIND_COUNT; i++) {
    if (strcmp(mode, kindNames[i]) == 0)
      kind = i;
  }
  if (kind < 0)
    return false;

  strncpy(id, sender, idSize - 1);
  id[idSize - 1] = '\0';

  state = RemoteState();
  state.kind = static_cast<RemoteState::Kind>(kind);
  state.running = doc["running"] | false;
  state.ms = doc["ms"] | 0L;
  state.blackMs = doc["black"] | 0L;
  state.white = doc["white"] | true;
  state.over = doc["over"] | false;
  return true;
}

bool remoteChanged(const RemoteState &a, const RemoteState &b) {
  if (a.kind != b.kind || a.running != b.running || a.white != b.white || a.over != b.over)
    return true;

  // While running the times change all the time, the heartbeat keeps them in sync
  return !a.running && (a.ms != b.ms || a.blackMs != b.blackMs);
}

int remoteSeconds(long ms) {
  if (ms > 0)
    return (ms + 999) / 1000;
  return -((-ms) / 1000);
}
