#include "proc.h"
#include "sig.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *updateProcesses(void *arg) {
    struct ProcessInfo *ps = (struct ProcessInfo *)arg;

    while (1) {
        getProcesses(ps);

        sleep(1);
    }

    return NULL;
}

int main(void) {
    pthread_t thread;

    struct ProcessInfo *processes = (struct ProcessInfo *)malloc(
        MAX_PROC_COUNT * sizeof(struct ProcessInfo));

    if (pthread_create(&thread, NULL, updateProcesses, (void *)processes) !=
        0) {
        perror("Failed to create thread");
        free(processes);
        return -1;
    }

    int input_pid, input_sig;
    while (1) {
        printf("\033[H\033[J");

        print_table(stdout, processes);

        if (readInput(&input_pid, &input_sig)) {
            struct signalThreadArgs args = {.pid = input_pid, .signal = input_sig};
            sendSignal(&args);
        }
    }

    if (pthread_join(thread, NULL) != 0) {
        perror("Failed to join thread");
    }

    free(processes);

    return 0;
}
