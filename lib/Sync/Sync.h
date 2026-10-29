#ifndef _SYNC_H_
#define _SYNC_H_

// Shares the active timer, stopwatch or chess clock with other devices on the
// network using UDP broadcast, and passes theirs to the mirror mode.
extern void syncSetup();
extern void syncLoop();

#endif
