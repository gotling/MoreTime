#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <Mode.h>
#include <Remote.h>

// Starts on open. Plus pauses and resumes, action resets or returns to the clock.
class StopWatchMode : public Mode {
public:
  StopWatchMode();
  const char *name() override { return "Chrono"; }
  void open() override;
  void close() override;
  void plus() override;
  void action() override;
  void update() override;

  // Control without buttons, e.g. from the phone. The stopwatch must be the current mode.
  void pause();
  void resume();

  // Running or paused stopwatch for other devices to mirror
  RemoteState remoteState();

  // Show elapsed time. Shared with the mirror.
  static void show(unsigned long elapsedMs);

private:
  bool running = false;
  unsigned long start = 0;
  unsigned long previous = 0;

  void startPause();
  bool started();
  void reset();
  void display();
};

extern StopWatchMode stopWatchMode;

#endif
