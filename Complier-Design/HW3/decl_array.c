#include "decl_array.h"
#include <stdlib.h>
#include "decl_item.h"

DeclArray_t* newDeclArray() {
    DeclArray_t* a = malloc(sizeof(DeclArray_t));
    a->capacity = INITIAL_CAPACITY;
    a->size = 0;
    a->arr = malloc(sizeof(DeclItem_t*) * a->capacity);
    return a;
}

DeclArray_t* pushDeclArray(DeclArray_t* a, DeclItem_t* val) {
    if (a->capacity < a->size + 1) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(DeclItem_t*) * a->capacity);
    }
    a->arr[a->size++] = val;
    return a;
}

void freeDeclArray(DeclArray_t* a) {
    if (a == NULL) return;
    for (int i = 0; i < a->size; i++) freeDeclItem(a->arr[i]);
    free(a->arr);
    free(a);
}
