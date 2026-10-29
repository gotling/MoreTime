#ifndef FAKE_DISPLAY_H
#define FAKE_DISPLAY_H

// Stand-in for lib/Display in native unit tests. Records what would be shown.

#include <stdarg.h>
#include <stdio.h>
#include <string>

enum FakeFont { FontDefault, FontWide, FontThin };

inline std::string displayText;
inline FakeFont displayFont = FontDefault;

inline void displayPrint(const char *text) {
  displayText = text;
}

inline void displayPrintf(const char *format, ...) {
  // Same size as the real display buffer, so truncation matches the device
  char buffer[16];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  displayText = buffer;
}

inline void displayFontWide() { displayFont = FontWide; }
inline void displayFontThin() { displayFont = FontThin; }
inline void displayFontDefault() { displayFont = FontDefault; }

#endif
