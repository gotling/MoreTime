#ifndef FAKE_ARDUINO_H
#define FAKE_ARDUINO_H

// Stand-in for the Arduino core in native unit tests.
// Time only moves when a test changes fakeMillis, see TestHelpers.h.

#include <algorithm>
#include <stdint.h>
#include <stdio.h>

using std::max;
using std::min;

inline unsigned long fakeMillis = 0;

inline unsigned long millis() {
  return fakeMillis;
}

struct FakeSerial {
  template <typename T> void print(T) {}
  template <typename T> void println(T) {}
  void println() {}
};

inline FakeSerial Serial;

#endif
