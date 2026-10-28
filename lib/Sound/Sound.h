#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdint.h>

#include <Mode.h>

// Pick the alarm sound. Minus/plus browse and preview, action saves and returns to time.
class SoundMode : public Mode {
public:
  const char *name() override { return "Sound"; }
  void open() override;
  void close() override;
  void minus() override;
  void plus() override;
  void action() override;

private:
  uint8_t index = 0;

  void preview();
  void display();
};

extern SoundMode soundMode;

#endif
