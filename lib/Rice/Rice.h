#ifndef _RICE_H_
#define _RICE_H_

#include <Mode.h>

// Rice calculator. Minus/plus set units of rice, the display alternates
// between units, grams of rice and grams of water.
class RiceMode : public Mode {
public:
  RiceMode();
  const char *name() override { return "Rice"; }
  void open() override;
  void close() override;
  void minus() override;
  void plus() override;
  void action() override;
  void update() override;

private:
  float rice = 0;
  unsigned short alternate = 1;

  void change(float amount);
};

extern RiceMode riceMode;

#endif
