#include <Display.h>
#include <Font.h>

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

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

void displayPrint(char* text) {
    myDisplay.print(text);
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