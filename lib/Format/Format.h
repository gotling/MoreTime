#ifndef _FORMAT_H_
#define _FORMAT_H_

// Text formatting shared by the modes. Free of hardware so it can be unit tested.
// Results are a shared buffer, so use only one result per displayPrintf.

// Format seconds as m:ss, minutes above 59 are not turned into hours
extern const char *formatMinutes(unsigned int seconds);
// Format seconds as h:mm:ss from one hour, otherwise m:ss
extern const char *formatDuration(unsigned int seconds);

#endif
