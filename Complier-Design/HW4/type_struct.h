#include "base.h"
#pragma once
#include "int_array.h"

typedef struct typestruct {
    Type_t type;
    IntArray_t* arr_sig;
} TypeStruct_t;

TypeStruct_t* newTypeStruct1(Type_t type);
TypeStruct_t* newTypeStruct2(Type_t type, IntArray_t* arr, int num);
bool eqType1(TypeStruct_t* tys, Type_t t);
bool eqType2(TypeStruct_t* t1, TypeStruct_t* t2);
void freeTypeStruct(TypeStruct_t* t);
