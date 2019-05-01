#include "table_stack.h"
#include <stdlib.h>

TableStack_t* newTableStack() {
    TableStack_t* ts = (TableStack_t*)malloc(sizeof(TableStack_t));
    ts->top = -1;
    ts->capacity = INITIAL_CAPACITY;
    ts->stack = (SymbolTable_t**)malloc(sizeof(SymbolTable_t*) * ts->capacity);
    return ts;
}

void pushTable(TableStack_t* ts) {
    if (ts->top + 1 >= ts->capacity) {
        ts->capacity *= 2;
        ts->stack = realloc(ts->stack, sizeof(SymbolTable_t*) * ts->capacity);
    }
    ts->stack[++ts->top] = newSymbolTable();
}

SymbolTable_t* getTopTable(TableStack_t* ts) {
    return ts->top >= 0 ? ts->stack[ts->top] : NULL;
}

void popTable(TableStack_t* ts) {
    freeSymbolTable(getTopTable(ts));
    ts->top--;
}
