#ifndef FAKE_MYTIME_H
#define FAKE_MYTIME_H

// Stand-in for the clock mode in native unit tests. Modes return here when
// they exit, so tests check modeCurrent() == &timeMode.

#include <Mode.h>

class TimeMode : public Mode {
public:
  const char *name() override { return "Time"; }
};

inline TimeMode timeMode;

#endif
