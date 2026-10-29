#include <Display.h>

#include <stdarg.h>
#include <string.h>

#include <MD_MAX72xx.h>
#include <MD_Parola.h>

#include <Font.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5   // or SCK
#define DATA_PIN D7  // or MOSI
#define CS_PIN D8    // or SS

#define BUF_SIZE 16
#define SCROLL_SIZE 32
// ms per column
#define SCROLL_SPEED 40

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

static char message[BUF_SIZE];
static char scroll[SCROLL_SIZE];

void displaySetup()
{
    myDisplay.begin();
    // Set the intensity (brightness) of the display (0-15):
    myDisplay.setIntensity(BRIGHTNESS);
    // Clear the display:
    myDisplay.displayClear();
    myDisplay.setFont(nullptr);
    myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print("TiME");
}

void displayLoop() {
    myDisplay.displayAnimate();
}

void displayPrint(const char *text) {
    myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print(text);
}

void displayPrintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(message, BUF_SIZE, format, args);
    va_end(args);

    displayPrint(message);
}

void displayScrollStart(const char *text) {
    strncpy(scroll, text, SCROLL_SIZE - 1);
    scroll[SCROLL_SIZE - 1] = '\0';
    myDisplay.displayText(scroll, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

bool displayScrollDone() {
    return myDisplay.getZoneStatus(0);
}

void displayScroll(const char *text) {
    displayScrollStart(text);
    while (!myDisplay.displayAnimate())
        yield();
}

void displayFontWide() {
    myDisplay.setFont(widefont);
}

void displayFontThin() {
    myDisplay.setFont(thinfont);
}

void displayFontDefault() {
    myDisplay.setFont(nullptr);
}

void displayBrightness(uint8_t intensity) {
    myDisplay.setIntensity(intensity);
}

void displayShutdown(bool off) {
    myDisplay.displayShutdown(off);
}
