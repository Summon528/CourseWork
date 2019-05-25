#pragma once
#include "base.h"
#include "type_struct.h"

typedef struct typearray {
    TypeStruct_t** arr;
    int size;
    int capacity;
} TypeArray_t;

TypeArray_t* newTypeArray();
TypeArray_t* pushTypeArray(TypeArray_t* a, TypeStruct_t* val);
void freeTypeArray(TypeArray_t* a);
