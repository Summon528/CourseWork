#include "decl_item.h"
#include <stdlib.h>

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
