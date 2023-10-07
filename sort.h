#ifndef SORT_H
#define SORT_H

#include <string.h>
#include <unistd.h>

int _strncmp(char *, char *, int);
void swap(char **, char **);

void quicksort(char *arr[], unsigned int length) {
    unsigned int i, piv = 0;
    if (length <= 1)
        return;

    for (i = 0; i < length; i++) {
        if (_strncmp(arr[i], arr[length - 1], 256) < 0)
            swap(arr + i, arr + piv++);
    }
    swap(arr + piv, arr + length - 1);

    quicksort(arr, piv++);
    quicksort(arr + piv, length - piv);
}

int _strncmp(char *a, char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] == '\0' && b[i] == '\0')
            return 0;

        if ('Z' >= a[i] && a[i] >= 'A' && 'z' >= b[i] && b[i] >= 'a')
            return 1;
        if ('z' >= a[i] && a[i] >= 'a' && 'Z' >= b[i] && b[i] >= 'A')
            return -1;

        if (a[i] - b[i])
            return a[i] - b[i];
    }

    return 0;
}

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

#endif
