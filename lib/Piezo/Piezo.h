#ifndef _PIEZO_H
#define _PIEZO_H

#include <stdint.h>

#define PIEZO_PIN D1

struct Note {
  uint16_t frequency;  // Hz, 0 for silence
  uint16_t length;     // ms
};

struct Sound {
  const char *name;
  const Note *notes;
  uint8_t count;
  bool decay;          // Fade each note out like a bell instead of a flat beep
  uint8_t repeat;      // Times to play when used as alarm
};

extern const Sound sounds[];
extern const uint8_t soundCount;

extern void piezoSetup();
extern void piezoLoop();
extern void piezoPlay();
extern void piezoPreview(uint8_t index);
extern void piezoStop();
extern void piezoIdle();
extern uint8_t piezoSound();
extern void piezoSetSound(uint8_t index);

#endif
