#include "table_stack.h"
#include <stdlib.h>

TableStack_t* newTableStack() {
    TableStack_t* ts = (TableStack_t*)malloc(sizeof(TableStack_t));
    ts->top = -1;
    ts->capacity = INITIAL_CAPACITY;
    ts->stack = (SymbolTable_t**)malloc(sizeof(SymbolTable_t*) * ts->capacity);
    return ts;
}

void pushTS(TableStack_t* ts) {
    if (ts->top + 1 >= ts->capacity) {
        ts->capacity *= 2;
        ts->stack = realloc(ts->stack, sizeof(SymbolTable_t*) * ts->capacity);
    }
    int next_var = getTopTS(ts) == NULL ? 0 : getTopTS(ts)->next_var;
    ts->stack[ts->top + 1] =
        newSymbolTable(ts->top == -1 ? 0 : getTopTS(ts)->level + 1, next_var);
    ts->top++;
}

SymbolTable_t* getTopTS(TableStack_t* ts) {
    return ts->top >= 0 ? ts->stack[ts->top] : NULL;
}

SymbolEntry_t* findTS(TableStack_t* ts, char* name) {
    for (int i = ts->top; i >= 0; i--) {
        SymbolEntry_t* target = findST(ts->stack[i], name);
        if (target != NULL) return target;
    }
    return NULL;
}

void popTS(TableStack_t* ts) {
    freeST(getTopTS(ts));
    ts->top--;
}

void freeTS(TableStack_t* ts) {
    while (ts->top != -1) popTS(ts);
    free(ts->stack);
    free(ts);
}
