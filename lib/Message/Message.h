#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <Arduino.h>

extern void messageSetup();
extern void messageLoop();
extern void messageOpen();
extern void messageClose();
extern void messageAdd(String &message);
extern void messageDisplay();
extern void messageMinus();
extern void messagePlus();
extern void messageAction();

#endif