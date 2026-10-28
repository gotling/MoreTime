#include <NTP.h>

#include <Arduino.h>
#include <time.h>

// Any time before 2020 means SNTP has not synced yet
#define MIN_VALID_TIME 1577836800

void ntpSetup() {
    // SNTP syncs in the background and the TZ rule handles daylight saving
    configTime(NTP_TIMEZONE, NTP_SERVER);
}

bool ntpTimeSet() {
    return time(nullptr) > MIN_VALID_TIME;
}

static struct tm ntpLocalTime() {
    time_t now = time(nullptr);
    struct tm local;
    localtime_r(&now, &local);
    return local;
}

int ntpHour() {
    return ntpLocalTime().tm_hour;
}

int ntpMinute() {
    return ntpLocalTime().tm_min;
}
