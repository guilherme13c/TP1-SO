#ifndef PROC_H
#define PROC_H

#define MAX_PROC_COUNT 20

struct ProcessInfo {
    int pid;
    char username[32];
    char procname[256];
    char state;
};

#endif