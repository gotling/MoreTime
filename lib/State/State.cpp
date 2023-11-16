#include <State.h>

#include <arduino-timer.h>

State state = {
    .mode = Menu,
    .counter = 0
};

char message[BUF_SIZE];
char oldMessage[BUF_SIZE];
