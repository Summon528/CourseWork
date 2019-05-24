#include "decl_item.h"
#include <stdlib.h>
#include <string.h>

DeclItem_t* newDeclItem(char* name, Type_t type, IntArray_t* arr_sig) {
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = name;
    di->arr_sig = arr_sig;
    di->type = type;
    return di;
}

DeclItem_t* copyDeclItem(DeclItem_t* other) {
    if (other == NULL) return NULL;
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = strdup(other->name);
    di->arr_sig = newIntArrayCpy(other->arr_sig);
    di->type = other->type;
    return di;
}

void freeDeclItem(DeclItem_t* di) {
    if (di == NULL) return;
    freeIntArray(di->arr_sig);
    free(di->name);
    free(di);
    return;
}

bool eqDeclItem(DeclItem_t* a, DeclItem_t* b) {
    if (a == NULL && b == NULL) return true;
    if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) return false;
    return a->type == b->type && eqIntArray(a->arr_sig, b->arr_sig);
}
