#ifndef _IP_H_
#define _IP_H_

#include <Mode.h>

// Scrolls the device's IP address, for opening the web page on a phone.
// Any button returns to the clock.
class IpMode : public Mode {
public:
  const char *name() override { return "IP"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;
  void update() override;

private:
  void scroll();
};

extern IpMode ipMode;

#endif
