#ifndef _HUM_H_
#define _HUM_H_

#include <Mode.h>

// Test mode for finding the source of an audible hum.
// Action cycles through the tests, minus/plus change the value, hold exits.
class HumMode : public Mode {
public:
  const char *name() override { return "Hum"; }
  void open() override;
  void close() override;
  void minus() override;
  void plus() override;
  void action() override;
  void update() override;

private:
  enum Test {
    Brightness = 0,
    WiFiSleep = 1,
    Display = 2,
    LoopDelay = 3,
    Piezo = 4
  };

  Test test = Brightness;
  int brightness;
  int wifiSleep;
  bool displayOn;
  int loopDelay;
  bool piezoLow;

  void defaults();
  void change(int direction);
  void apply();
  void display();
};

extern HumMode humMode;

#endif
