#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <Remote.h>

// Runs commands from outside the device, like the phone web page, without
// knowing about the network. Commands are "timer/start" with seconds,
// "timer/pause", "timer/resume", "timer/stop" and the same for "stopwatch/",
// where stopwatch/start takes no seconds and always starts from zero.

enum ControlResult {
  ControlOk,
  ControlUnknown,   // No such command
  ControlInvalid,   // Seconds missing or out of range
  ControlInactive   // Pause, resume or stop for something not on screen
};

#define CONTROL_MAX_SECONDS (24L * 3600)

extern ControlResult controlRun(const char *command, long seconds);
// The timer, stopwatch or chess clock running on this device, if any
extern RemoteState controlState();

#endif
