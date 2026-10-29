#ifndef _MIRROR_H_
#define _MIRROR_H_

#include <Mode.h>
#include <Remote.h>

// Shows a timer, stopwatch or chess clock running on another device.
// Takes over from the clock when one starts and returns when it ends or
// the other device goes quiet. Any button dismisses it until the next one.
class MirrorMode : public Mode {
public:
  MirrorMode();
  const char *name() override { return "Mirror"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;
  void hold() override;
  void update() override;

  // State received from another device
  void receive(const RemoteState &state);

private:
  RemoteState state;
  unsigned long receivedAt = 0;
  // Kind the user dismissed, ignored until the other device stops it
  RemoteState::Kind dismissed = RemoteState::None;
  // For sounding the alarm when the timer passes zero, not when joining late
  bool timerLeft = false;

  long elapsed();
  void dismiss();
  void display();
};

extern MirrorMode mirrorMode;

#endif
