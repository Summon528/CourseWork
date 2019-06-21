#pragma once
#include "base.h"
#include "param_array.h"
#include "symbol_entry.h"

typedef struct symboltable {
    int level;
    SymbolEntry_t** entries;
    int size;
    int capacity;
    int next_var;
} SymbolTable_t;

SymbolTable_t* newSymbolTable(int level, int next_var);
void pushSTParamArray(SymbolTable_t* st, ParamArray_t* da);
SymbolEntry_t* pushSTFunc(SymbolTable_t* st, char* name, Type_t type,
                          ParamArray_t* da, int decl);
SymbolEntry_t* findST(SymbolTable_t* st, char* name);
void printST(SymbolTable_t* st);
void freeST(SymbolTable_t* st);
void pushSTConst(SymbolTable_t* st, char* name, Literal_t* lit);
void pushSTVar(SymbolTable_t* st, char* name, IntArray_t* arr);
