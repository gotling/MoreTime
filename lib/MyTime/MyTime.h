#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <Mode.h>

// The clock, shown at startup and when leaving other modes
class TimeMode : public Mode {
public:
  TimeMode();
  const char *name() override { return "Time"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;
  void hold() override;
  void update() override;
};

extern TimeMode timeMode;

#endif
