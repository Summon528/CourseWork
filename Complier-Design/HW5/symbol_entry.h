#pragma once
#include "base.h"
#include "int_array.h"
#include "literal.h"
#include "param_array.h"

typedef struct SymbolEntry_t {
    char name[64];
    Kind_t kind;
    Type_t type;

    IntArray_t* arr_sig;
    Literal_t* const_val;
    ParamArray_t* params;

    bool fun_defed;
    int var_num;
} SymbolEntry_t;

SymbolEntry_t* newSymbolEntry();
void printSymbolEntry(SymbolEntry_t* se, int level);
void freeSymbolEntry(SymbolEntry_t* se);
