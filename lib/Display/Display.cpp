#include <Display.h>

#include <stdarg.h>

#include <MD_MAX72xx.h>
#include <MD_Parola.h>

#include <Font.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5   // or SCK
#define DATA_PIN D7  // or MOSI
#define CS_PIN D8    // or SS

#define BUF_SIZE 16

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

static char message[BUF_SIZE];

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
