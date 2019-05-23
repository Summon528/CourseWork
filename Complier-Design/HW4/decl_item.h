#pragma once
#include "int_array.h"
#include "literal.h"

typedef struct declitem {
    char* name;
    IntArray_t* arr_sig;
    Literal_t* val;
    Type_t type;
} DeclItem_t;

DeclItem_t* newArrDecl(char* name, IntArray_t* arr_sig);
DeclItem_t* newDeclItem(char* name);
DeclItem_t* newDeclItemConst(char* name, Literal_t* lit);
DeclItem_t* newDeclItemParam(char* name, Type_t type);
DeclItem_t* newDeclItemCopy(DeclItem_t* other);
void freeDeclItem(DeclItem_t* di);
