#ifndef SORT_H
#define SORT_H

#include <string.h>
#include <unistd.h>

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(char *arr[], unsigned int length) {
    unsigned int i, piv = 0;
    if (length <= 1)
        return;

    for (i = 0; i < length; i++) {
        if (strcmp(arr[i], arr[length - 1]) < 0)
            swap(arr + i, arr + piv++);
    }
    swap(arr + piv, arr + length - 1);

    quicksort(arr, piv++);
    quicksort(arr + piv, length - piv);
}

#endif
