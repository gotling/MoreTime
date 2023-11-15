#include <MD_MAX72xx.h>
#include <MD_Parola.h>

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5   // or SCK
#define DATA_PIN D7  // or MOSI
#define CS_PIN D8    // or SS

#define CHAR_SPACING 1    // Pixels between characters
#define USE_LOCAL_FONT 1  // Use font from Font_Data.h
#define BRIGHTNESS 2

extern void displaySetup();
extern void displayPrint(char* text);
extern void displayAlign(enum textPosition_t);
extern void displayFontWide();
extern void displayFontThin();
extern void displayFontDefault();

#endif