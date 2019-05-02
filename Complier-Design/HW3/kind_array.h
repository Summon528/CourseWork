#pragma once
#include "base.h"

typedef struct kindarray {
    int* arr;
    int size;
    int capacity;
} KindArray_t;

KindArray_t* newKindArray();
KindArray_t* pushKindArray(KindArray_t* a, Kind_t val);
void freeKindArray(KindArray_t* a);
