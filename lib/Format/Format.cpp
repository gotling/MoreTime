#include <Format.h>

#include <stdio.h>

#define FORMAT_SIZE 16

static char buffer[FORMAT_SIZE];

const char *formatMinutes(unsigned int seconds) {
  snprintf(buffer, FORMAT_SIZE, "%u:%02u", seconds / 60, seconds % 60);
  return buffer;
}

const char *formatDuration(unsigned int seconds) {
  if (seconds < 3600)
    return formatMinutes(seconds);

  snprintf(buffer, FORMAT_SIZE, "%u:%02u:%02u", seconds / 3600, (seconds % 3600) / 60, seconds % 60);
  return buffer;
}
