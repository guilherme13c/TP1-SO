#include "proc.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    struct ProcessInfo *processes = (struct ProcessInfo *)malloc(
        MAX_PROC_COUNT * sizeof(struct ProcessInfo));

    

    free(processes);

    return EXIT_SUCCESS;
}
