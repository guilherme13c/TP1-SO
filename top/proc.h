#ifndef PROC_H
#define PROC_H

#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PROC_COUNT 20

struct ProcessInfo {
    int pid;
    char username[32];
    char procname[256];
    char state;
};

void getProcesses(struct ProcessInfo *ps) {
    DIR *dir;
    FILE *fp;
    struct dirent *entry;
    struct stat filestat;
    int count = 0;
    char filepath[1024];
    char buffer[1024];

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Unable to open /proc");
        return;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_PROC_COUNT) {
        if (atoi(entry->d_name) != 0) {

            // Get PID
            ps[count].pid = atoi(entry->d_name);

            // Get username
            sprintf(filepath, "/proc/%s", entry->d_name);
            if (stat(filepath, &filestat) == -1) {
                perror("Unable to stat file");
                return;
            }
            struct passwd *pwd = getpwuid(filestat.st_uid);
            strncpy(ps[count].username, pwd->pw_name,
                    sizeof(ps[count].username));

            // Get process name
            sprintf(filepath, "/proc/%s/comm", entry->d_name);
            fp = fopen(filepath, "r");
            if (fp == NULL) {
                perror("Unable to open file");
                return;
            }

            fgets(buffer, sizeof(buffer), fp);
            buffer[strlen(buffer) - 1] = '\0'; // Remove newline
            strncpy(ps[count].procname, buffer, sizeof(ps[count].procname));
            fclose(fp);

            // Get process state
            sprintf(filepath, "/proc/%s/stat", entry->d_name);
            fp = fopen(filepath, "r");
            if (fp == NULL) {
                perror("Unable to open file");
                return;
            }

            fgets(buffer, sizeof(buffer), fp);
            char *token = strtok(buffer, " "); // PID
            token = strtok(NULL, " ");         // Comm
            token = strtok(NULL, " ");         // State
            ps[count].state = token[0];
            fclose(fp);

            count++;
        }
    }

    closedir(dir);
}

void print_table(FILE *stream, struct ProcessInfo *ps) {
    fprintf(stdout, "%-6s | %-15s | %-30s | %-6s |\n", "PID", "User",
            "PROCNAME", "Estado");
    fprintf(stdout,
            "-------------------------------------------------------------------|\n");

    for (int i = 0; i < MAX_PROC_COUNT; i++) {
        fprintf(stdout, "%-6d | %-15s | %-30s | %-6c |\n", ps[i].pid, ps[i].username, ps[i].procname, ps[i].state);
    }
    fprintf(stdout,
            "-------------------------------------------------------------------|\n");
}

#endif