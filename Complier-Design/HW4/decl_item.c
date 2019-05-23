#include "decl_item.h"
#include <stdlib.h>
#include <string.h>

DeclItem_t* newArrDecl(char* name, IntArray_t* arr_sig) {
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = name;
    di->arr_sig = arr_sig;
    di->val = NULL;
    return di;
}

DeclItem_t* newDeclItem(char* name) {
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = name;
    di->arr_sig = NULL;
    di->val = NULL;
    return di;
}

DeclItem_t* copyDeclItem(DeclItem_t* other) {
    if (other == NULL) return NULL;
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = strdup(other->name);
    di->arr_sig = newIntArrayCpy(other->arr_sig);
    di->val = copyLiteral(other->val);
    di->type = other->type;
    return di;
}

DeclItem_t* newDeclItemConst(char* name, Literal_t* lit) {
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = name;
    di->arr_sig = NULL;
    di->val = lit;
    return di;
}

DeclItem_t* newDeclItemParam(char* name, Type_t type) {
    DeclItem_t* di = malloc(sizeof(DeclItem_t));
    di->name = name;
    di->arr_sig = NULL;
    di->val = NULL;
    di->type = type;
    return di;
}

void freeDeclItem(DeclItem_t* di) {
    if (di == NULL) return;
    freeIntArray(di->arr_sig);
    free(di->val);
    free(di->name);
    free(di);
    return;
}

bool eqDeclItem(DeclItem_t* a, DeclItem_t* b) {
    if (a == NULL && b == NULL) return true;
    if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) return false;
    return a->type == b->type && eqIntArray(a->arr_sig, b->arr_sig);
}
