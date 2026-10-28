#ifndef _NTP_H_
#define _NTP_H_

#include <TZ.h>

#define NTP_TIMEZONE TZ_Europe_Stockholm
#define NTP_SERVER "europe.pool.ntp.org"

extern void ntpSetup();
extern bool ntpTimeSet();
extern int ntpHour();
extern int ntpMinute();

#endif
