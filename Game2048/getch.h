#ifndef GETCH_H
#define GETCH_H

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error (const char* msg) {
    perror(msg);
    exit(-1);
}

char _getch() {
    int c = 0, res = 0;
    struct termios org_opts, new_opts;
    if ((res = tcgetattr(STDIN_FILENO, &org_opts))) {
        error("getattr");
    }
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    if ((res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts))) {
        error("setattr");
    }
    return c;
}

#endif
