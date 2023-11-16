#ifndef _NTP_H_
#define _NTP_H_

extern void ntpSetup();
extern void ntpLoop();
extern bool ntpTimeSet();
extern int ntpHour();
extern int ntpMinute();

#endif