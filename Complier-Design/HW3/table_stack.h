#pragma once
#include "base.h"
#include "symbol_table.h"

typedef struct TableStack_t {
    SymbolTable_t** stack;
    int top;
    int capacity;
} TableStack_t;

TableStack_t* newTableStack();
void pushTS(TableStack_t* ts);
void popTS(TableStack_t* ts);
SymbolTable_t* getTopTS(TableStack_t* ts);
void freeTS(TableStack_t* ts);
