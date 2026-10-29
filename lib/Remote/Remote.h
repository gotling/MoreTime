#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <stddef.h>

// What a device's timer, stopwatch or chess clock is doing, sent to other
// devices so they can mirror it. Times are in ms at the moment of sending.
struct RemoteState {
  enum Kind { None, Timer, StopWatch, Chess };

  Kind kind = None;
  bool running = false;
  long ms = 0;          // Timer: time left, StopWatch: elapsed, Chess: white's time left
  long blackMs = 0;     // Chess: black's time left
  bool white = true;    // Chess: white to move
  bool over = false;    // Chess: game over, the player to move ran out of time
};

// Serialize to JSON, returns false if it did not fit
extern bool remoteEncode(const RemoteState &state, const char *id, char *buffer, size_t size);
// Parse JSON from another device. id receives the sender, at most idSize - 1 characters.
extern bool remoteDecode(const char *json, RemoteState &state, char *id, size_t idSize);
// True if the change is worth sending right away instead of waiting for the next heartbeat
extern bool remoteChanged(const RemoteState &a, const RemoteState &b);
// Seconds as shown on the display, which rounds up so 0:00 means time is up
extern int remoteSeconds(long ms);

#endif
