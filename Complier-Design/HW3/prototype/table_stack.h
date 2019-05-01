#pragma once
#include "symbol_table.h"
#define INITIAL_CAPACITY 4

typedef struct TableStack_t {
    SymbolTable_t** stack;
    int top;
    int capacity;
} TableStack_t;

TableStack_t* newTableStack();
void pushTable(TableStack_t* ts);
void popTable(TableStack_t* ts);
SymbolTable_t* getTopTable(TableStack_t* ts);
