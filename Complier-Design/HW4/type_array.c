#include "type_array.h"
#include <stdlib.h>

TypeArray_t* newTypeArray() {
    TypeArray_t* a = malloc(sizeof(TypeArray_t));
    a->capacity = INITIAL_CAPACITY;
    a->size = 0;
    a->arr = malloc(sizeof(TypeStruct_t*) * a->capacity);
    return a;
}

TypeArray_t* pushTypeArray(TypeArray_t* a, TypeStruct_t* val) {
    if (a->capacity < a->size + 1) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(TypeStruct_t*) * a->capacity);
    }
    a->arr[a->size++] = val;
    return a;
}

void freeTypeArray(TypeArray_t* a) {
    if (a == NULL) return;
    for (int i = 0; i < a->size; i++) freeTypeStruct(a->arr[i]);
    free(a->arr);
    free(a);
}
