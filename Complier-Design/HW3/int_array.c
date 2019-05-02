#include "int_array.h"
#include <stdlib.h>
#include <string.h>

IntArray_t* newIntArray() {
    IntArray_t* a = malloc(sizeof(IntArray_t));
    a->capacity = INITIAL_CAPACITY;
    a->size = 0;
    a->arr = malloc(sizeof(int) * a->capacity);
    return a;
}

IntArray_t* newIntArrayCpy(IntArray_t* other) {
    if (other == NULL) return NULL;
    IntArray_t* a = malloc(sizeof(IntArray_t));
    a->capacity = other->capacity;
    a->size = other->size;
    a->arr = malloc(sizeof(int) * a->capacity);
    memcpy(a->arr, other->arr, sizeof(int) * a->size);
    return a;
}

IntArray_t* pushIntArray(IntArray_t* a, int val) {
    if (a->capacity < a->size + 1) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(int) * a->capacity);
    }
    a->arr[a->size++] = val;
    return a;
}

void freeIntArray(IntArray_t* a) {
    if (a == NULL) return;
    free(a->arr);
    free(a);
}
