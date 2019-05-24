#pragma once
#include "base.h"
#include "param_item.h"

typedef struct paramarray {
    ParamItem_t** arr;
    int size;
    int capacity;
} ParamArray_t;

ParamArray_t* newParamArray();
ParamArray_t* copyParamArray(ParamArray_t* other);
ParamArray_t* pushDeclArray(ParamArray_t* a, ParamItem_t* val);
void freeParamArray(ParamArray_t* a);
bool eqParamArray(ParamArray_t* a, ParamArray_t* b);
