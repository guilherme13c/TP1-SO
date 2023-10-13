#ifndef SIG_H
#define SIG_H

#include "proc.h"
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>

struct signalThreadArgs {
    pid_t pid;
    int signal;
};

void *sendSignal(void *arg) {
    struct signalThreadArgs *args = (struct signalThreadArgs *)arg;

    if (kill(args->pid, args->signal) != 0) {
        perror("Error sending signal");
    }

    return NULL;
}

int readInput(int *pid, int *sig) {
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);   /* clear the set */
    FD_SET(0, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; // 1ms timeout interval

    int rv = select(1, &set, NULL, NULL, &timeout);
    if (rv == -1)
        perror("select"); /* an error accured */
    else if (rv == 0)
        return 0;
    else {
        scanf("> %d %d", pid, sig);
        return 1;
    }
}

#endif