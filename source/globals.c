#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include "globals.h"

struct settings storedSettings = {0};

int min(const int *arr, size_t n) {
    int m = 0;
    for (size_t i = 0; i < n; ++i) if (arr[i] < m) m = arr[i];
    return m;
}

int max(const int *arr, size_t n) {
    int m = 0;
    for (size_t i = 0; i < n; ++i) if (arr[i] > m) m = arr[i];
    return m;
}

bool any(bool array[], int size) {
    bool res = false;
    for (int i = 0; i < size; i++) {
        if (array[i]) {
            res = true;
            break;
        }
    }
    return res;
}

bool all(bool array[], int size) {
    bool res = true;
    for (int i = 0; i < size; i++) {
        if (!array[i]) {
            res = false;
            break;
        }
    }
    return res;
}

bool startsWith(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}