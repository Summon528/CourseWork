#pragma once
#include "int_array.h"
#include "literal.h"

typedef struct paramitem {
    char* name;
    IntArray_t* arr_sig;
    Type_t type;
} ParamItem_t;

ParamItem_t* newParamItem(char* name, Type_t type, IntArray_t* arr_sig);
ParamItem_t* copyParamItem(ParamItem_t* other);
void freeParamItem(ParamItem_t* di);
bool eqParamItem(ParamItem_t* a, ParamItem_t* b);
