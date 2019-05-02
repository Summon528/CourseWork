#pragma once
#include "int_array.h"
#include "literal.h"

typedef struct declitem {
    char* name;
    IntArray_t* arr_sig;
    Literal_t* val;
} DeclItem_t;

DeclItem_t* newArrDecl(char* name, IntArray_t* arr_sig);
DeclItem_t* newDeclItem(char* name);
void freeDeclItem(DeclItem_t* di);
