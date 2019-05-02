#pragma once
#include "base.h"
#include "decl_item.h"

typedef struct declarray {
    DeclItem_t** arr;
    int size;
    int capacity;
} DeclArray_t;

DeclArray_t* newDeclArray();
DeclArray_t* pushDeclArray(DeclArray_t* a, DeclItem_t* val);
void freeDeclArray(DeclArray_t* a);
