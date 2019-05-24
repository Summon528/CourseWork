#include "param_array.h"
#include <stdlib.h>
#include "param_item.h"

ParamArray_t* newParamArray() {
    ParamArray_t* a = malloc(sizeof(ParamArray_t));
    a->capacity = INITIAL_CAPACITY;
    a->size = 0;
    a->arr = malloc(sizeof(ParamItem_t*) * a->capacity);
    return a;
}

ParamArray_t* copyParamArray(ParamArray_t* other) {
    if (other == NULL) return NULL;
    ParamArray_t* a = malloc(sizeof(ParamArray_t));
    a->capacity = other->capacity;
    a->size = 0;
    a->arr = malloc(sizeof(ParamItem_t*) * a->capacity);
    for (int i = 0; i < other->size; i++)
        pushDeclArray(a, copyParamItem(other->arr[i]));
    return a;
}

ParamArray_t* pushDeclArray(ParamArray_t* a, ParamItem_t* val) {
    if (a->capacity < a->size + 1) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(ParamItem_t*) * a->capacity);
    }
    a->arr[a->size++] = val;
    return a;
}

void freeParamArray(ParamArray_t* a) {
    if (a == NULL) return;
    for (int i = 0; i < a->size; i++) freeParamItem(a->arr[i]);
    free(a->arr);
    free(a);
}

bool eqParamArray(ParamArray_t* a, ParamArray_t* b) {
    if (a == NULL && b == NULL) return true;
    if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) return false;
    if (a->size != b->size) return false;
    for (int i = 0; i < a->size; i++) {
        if (!eqParamItem(a->arr[i], b->arr[i])) return false;
    }
    return true;
}
