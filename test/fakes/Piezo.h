#ifndef FAKE_PIEZO_H
#define FAKE_PIEZO_H

// Stand-in for lib/Piezo in native unit tests. Counts alarm calls.

inline int piezoPlays = 0;

inline void piezoPlay() {
  piezoPlays++;
}

#endif
