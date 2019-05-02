#include "kind_array.h"
#include <stdlib.h>

KindArray_t* newKindArray() {
    KindArray_t* a = malloc(sizeof(KindArray_t));
    a->capacity = INITIAL_CAPACITY;
    a->size = 0;
    a->arr = malloc(sizeof(int) * a->capacity);
    return a;
}

KindArray_t* pushKindArray(KindArray_t* a, Kind_t val) {
    if (a->capacity < a->size + 1) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(int) * a->capacity);
    }
    a->arr[a->size++] = val;
    return a;
}

void freeKindArray(KindArray_t* a) {
    if (a == NULL) return;
    free(a->arr);
    free(a);
}
