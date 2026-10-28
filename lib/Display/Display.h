#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>

#define BRIGHTNESS 2

extern void displaySetup();
extern void displayLoop();
extern void displayPrint(const char *text);
// Like printf, output longer than the display buffer is cut off
extern void displayPrintf(const char *format, ...) __attribute__((format(printf, 1, 2)));
extern void displayFontWide();
extern void displayFontThin();
extern void displayFontDefault();
extern void displayBrightness(uint8_t intensity);
extern void displayShutdown(bool off);

// Format seconds as m:ss, minutes above 59 are not turned into hours.
// Returns a shared buffer, so use only one result per displayPrintf.
extern const char *formatMinutes(unsigned int seconds);
// Format seconds as h:mm:ss from one hour, otherwise m:ss
extern const char *formatDuration(unsigned int seconds);

#endif
