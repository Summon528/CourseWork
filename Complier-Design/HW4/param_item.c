#include "param_item.h"
#include <stdlib.h>
#include <string.h>

ParamItem_t* newParamItem(char* name, Type_t type, IntArray_t* arr_sig) {
    ParamItem_t* di = malloc(sizeof(ParamItem_t));
    di->name = name;
    di->arr_sig = arr_sig;
    di->type = type;
    return di;
}

ParamItem_t* copyParamItem(ParamItem_t* other) {
    if (other == NULL) return NULL;
    ParamItem_t* di = malloc(sizeof(ParamItem_t));
    di->name = strdup(other->name);
    di->arr_sig = newIntArrayCpy(other->arr_sig);
    di->type = other->type;
    return di;
}

void freeParamItem(ParamItem_t* di) {
    if (di == NULL) return;
    freeIntArray(di->arr_sig);
    free(di->name);
    free(di);
    return;
}

bool eqParamItem(ParamItem_t* a, ParamItem_t* b) {
    if (a == NULL && b == NULL) return true;
    if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) return false;
    return a->type == b->type && eqIntArray(a->arr_sig, b->arr_sig);
}
