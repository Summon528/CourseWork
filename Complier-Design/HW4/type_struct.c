#include "type_struct.h"
#include <stdlib.h>

TypeStruct_t* newTypeStruct1(Type_t type) {
    TypeStruct_t* tys = (TypeStruct_t*)malloc(sizeof(TypeStruct_t));
    tys->type = type;
    tys->arr_sig = NULL;
    return tys;
}

TypeStruct_t* newTypeStruct2(Type_t type, IntArray_t* arr, int num) {
    TypeStruct_t* tys = (TypeStruct_t*)malloc(sizeof(TypeStruct_t));
    tys->type = type;
    tys->arr_sig = copyIntArray(arr, num);
    return tys;
}

bool eqType1(TypeStruct_t* tys, Type_t t) {
    return tys->arr_sig == NULL && tys->type == t;
}

bool eqType2(TypeStruct_t* t1, TypeStruct_t* t2) {
    return t1->type == t2->type && eqIntArray(t1->arr_sig, t2->arr_sig);
}

void freeTypeStruct(TypeStruct_t* t) {
    freeIntArray(t->arr_sig);
    free(t);
}
