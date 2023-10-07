#ifndef MYEXEC_H
#define MYEXEC_H

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

void _ls(char *, int);
void _sort(char **);
void _wc(char **);
void _cat(char **);
void _echo(char **);

void runexec(char **argv) {
    if (strncmp(argv[0], "ls", 3) == 0) {
        _ls(".", 0);
    } else if (strncmp(argv[0], "wc", 3) == 0) {
        _wc(argv);
    } else if (strncmp(argv[0], "cat", 4) == 0) {
        _cat(argv);
    } else if (strncmp(argv[0], "echo", 5) == 0) {
        _echo(argv);
    } else {
        fprintf(stderr, "exec nao implementado\n");
    }
}

void _ls(char *dir, int op_a) {
    struct dirent *d;
    DIR *dh = opendir(dir);

    if (!dh) {
        if (errno == ENOENT) {
            fprintf(stderr, "directory does not exist\n");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "unable to read directory\n");
        exit(EXIT_FAILURE);
    }

    int c = 0;

    while ((d = readdir(dh)) != NULL) {
        c++;
    }

    dh = opendir(dir);

    if (!dh) {
        if (errno == ENOENT) {
            fprintf(stderr, "directory does not exist\n");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "unable to read directory\n");
        exit(EXIT_FAILURE);
    }

    char **files = (char **)malloc(c * sizeof(char *));
    for (int i = 0; i < c; i++)
        files[i] = (char *)malloc(256 * sizeof(char));

    int i = 0;
    while ((d = readdir(dh)) != NULL) {
        strncpy(files[i], d->d_name, 256);
        i++;
    }

    quicksort(files, c);

    if (op_a) {
        for (int i = 0; i < c; i++) {
            printf("%s\n", files[i]);
        }
    } else {
        for (int i = 0; i < c; i++) {
            if (files[i][0] != '.')
                printf("%s\n", files[i]);
        }
    }

    free(files);
}

/*
void _sort(char **argv) {
    quicksort(lines, length);

    for (int i = 0; i < length; i++) {
        printf("%s\n", lines[i]);
    }
}
*/

void _wc(char **argv) {
    int argc = 0;
    while (argv[++argc] != NULL)
        ;

    if (argc <= 1) {
        fprintf(stderr,
                "nao ha argumentos suficientes para executar o comando wc\n");
        return;
    }

    unsigned long totalWords = 0, totalLines = 0, totalBytes = 0;
    unsigned long words = 0, lines = 0, bytes = 0;
    int inWord = 0;

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL) {
            fprintf(stderr, "nao foi possivel abrir o arquivo \'%s\'\n",
                    argv[i]);
            return;
        }

        int c;
        while ((c = fgetc(fp)) != EOF) {
            bytes++;

            if (c == '\n')
                lines++;

            if (c == ' ') {
                inWord = 0;
            } else if (!inWord) {
                inWord = 1;
                words++;
            }
        }

        totalBytes += bytes;
        totalLines += lines;
        totalWords += words;

        printf(" %lu %lu %lu %s\n", lines, words, bytes, argv[i]);

        bytes = 0;
        lines = 0;
        words = 0;

        fclose(fp);
    }

    if (argc > 2)
        printf("%lu %lu %lu total\n", totalLines, totalWords, totalBytes);
}

void _cat(char **argv) {
    int argc = 0;
    while (argv[++argc] != NULL)
        ;

    if (argc <= 1) {
        fprintf(stderr,
                "nao ha argumentos suficientes para executar o comando cat\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL) {
            fprintf(stderr, "nao foi possivel abrir o arquivo \'%s\'\n",
                    argv[i]);
            return;
        }

        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char *buffer = (char *)malloc(fsize + 1);

        if (buffer == NULL) {
            fprintf(stderr, "memory allocation failed\n");
            fclose(fp);
            return;
        }

        size_t bytes_read = fread(buffer, 1, fsize, fp);
        buffer[bytes_read] = '\0';

        if (bytes_read != fsize) {
            fprintf(stderr, "error reading file \'%s\'\n", argv[i]);
        } else {
            printf("%s", buffer);
        }

        free(buffer);
        fclose(fp);
    }
}

void _echo(char **argv) {
    int c = 1;
    while (argv[c] != NULL)
        printf("%s ", argv[c++]);

    printf("\n");
}

#endif
