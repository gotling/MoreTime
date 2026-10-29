#ifndef _WEB_H_
#define _WEB_H_

// Web page and API for controlling the timer and stopwatch from a phone.
// GET / is the page, GET /api/state the current state, and POST /api/<command>
// runs a command from Control.h, e.g. POST /api/timer/start?seconds=300.
extern void webSetup();
extern void webLoop();

#endif
