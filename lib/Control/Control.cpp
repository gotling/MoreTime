#include <Control.h>

#include <string.h>

#include <Mode.h>
#include <MyTime.h>
#include <Timer.h>
#include <StopWatch.h>
#include <Chess.h>

static ControlResult timerCommand(const char *action, long seconds) {
  if (strcmp(action, "start") == 0) {
    if (seconds < 1 || seconds > CONTROL_MAX_SECONDS)
      return ControlInvalid;
    if (modeCurrent() != &timerMode)
      modeSwitch(&timerMode);
    timerMode.start(seconds);
    return ControlOk;
  }

  bool known = strcmp(action, "pause") == 0 || strcmp(action, "resume") == 0 || strcmp(action, "stop") == 0;
  if (!known)
    return ControlUnknown;
  if (modeCurrent() != &timerMode)
    return ControlInactive;

  if (strcmp(action, "pause") == 0)
    timerMode.pause();
  else if (strcmp(action, "resume") == 0)
    timerMode.resume();
  else
    modeSwitch(&timeMode);
  return ControlOk;
}

static ControlResult stopWatchCommand(const char *action) {
  if (strcmp(action, "start") == 0) {
    // Opening the stopwatch starts it from zero, also when already open
    modeSwitch(&stopWatchMode);
    return ControlOk;
  }

  bool known = strcmp(action, "pause") == 0 || strcmp(action, "resume") == 0 || strcmp(action, "stop") == 0;
  if (!known)
    return ControlUnknown;
  if (modeCurrent() != &stopWatchMode)
    return ControlInactive;

  if (strcmp(action, "pause") == 0)
    stopWatchMode.pause();
  else if (strcmp(action, "resume") == 0)
    stopWatchMode.resume();
  else
    modeSwitch(&timeMode);
  return ControlOk;
}

ControlResult controlRun(const char *command, long seconds) {
  if (strncmp(command, "timer/", 6) == 0)
    return timerCommand(command + 6, seconds);
  if (strncmp(command, "stopwatch/", 10) == 0)
    return stopWatchCommand(command + 10);
  return ControlUnknown;
}

RemoteState controlState() {
  Mode *mode = modeCurrent();
  if (mode == &timerMode)
    return timerMode.remoteState();
  if (mode == &stopWatchMode)
    return stopWatchMode.remoteState();
  if (mode == &chessMode)
    return chessMode.remoteState();
  return RemoteState();
}
