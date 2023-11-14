#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 600000);

void ntpSetup() {
    timeClient.begin();
}

void ntpLoop() {
    timeClient.update();
}

int ntpHour() {
    return timeClient.getHours();
}

int ntpMinute() {
    return timeClient.getMinutes();
}