#ifndef _PIEZO_H
#define _PIEZO_H

#define PIEZO_PIN D1

extern void piezoSetup();
extern void piezoLoop();
extern void piezoPlay();
extern void piezoStop();

#endif