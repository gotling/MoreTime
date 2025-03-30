#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 600000);

void ntpSetup() {
    timeClient.begin();
}

void ntpLoop() {
    timeClient.update();
}

bool ntpTimeSet() {
    return timeClient.isTimeSet();
}

int ntpHour() {
    return timeClient.getHours();
}

int ntpMinute() {
    return timeClient.getMinutes();
}