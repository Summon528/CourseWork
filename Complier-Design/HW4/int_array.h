#pragma once
#include <stdbool.h>
#include "base.h"

typedef struct intarray {
    int* arr;
    int size;
    int capacity;
} IntArray_t;

IntArray_t* newIntArray();
IntArray_t* newIntArrayCpy(IntArray_t* other);
IntArray_t* pushIntArray(IntArray_t* a, int val);
void freeIntArray(IntArray_t* a);
bool eqIntArray(IntArray_t* a, IntArray_t* b);
