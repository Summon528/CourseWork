#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

SymbolTable_t* newSymbolTable(int level) {
    SymbolTable_t* st = malloc(sizeof(SymbolTable_t));
    st->level = level;
    st->size = 0;
    st->capacity = INITIAL_CAPACITY;
    st->entries = malloc(sizeof(SymbolEntry_t*) * st->capacity);
    return st;
}

SymbolEntry_t* pushST(SymbolTable_t* st, char* name) {
    if (findST(st, name) != NULL) {
        fprintf(stderr,
                "##########Error at Line #%d: %s redeclared.##########\n",
                linenum, name);
        return NULL;
    }
    SymbolEntry_t* se = newSymbolEntry();
    if (st->capacity < st->size + 1) {
        st->capacity *= 2;
        st->entries =
            realloc(st->entries, sizeof(SymbolEntry_t*) * st->capacity);
    }
    strncpy(se->name, name, sizeof(se->name) - 1);
    st->entries[st->size++] = se;
    return se;
}

void pushSTVarArray(SymbolTable_t* st, DeclArray_t* da, Type_t type) {
    for (int i = 0; i < da->size; i++) {
        SymbolEntry_t* se = pushST(st, da->arr[i]->name);
        if (se == NULL) continue;
        se->kind = variable;
        se->arr_sig = newIntArrayCpy(da->arr[i]->arr_sig);
        se->type = type;
    }
}

void pushSTConstArray(SymbolTable_t* st, DeclArray_t* da, Type_t type) {
    for (int i = 0; i < da->size; i++) {
        SymbolEntry_t* se = pushST(st, da->arr[i]->name);
        if (se == NULL) continue;
        se->kind = constant;
        se->const_val = newLiteralCopy(da->arr[i]->val);
        se->type = type;
    }
}

SymbolEntry_t* findST(SymbolTable_t* st, char* name) {
    for (int i = 0; i < st->size; i++) {
        if (strcmp(st->entries[i]->name, name) == 0) {
            return st->entries[i];
        }
    }
    return NULL;
}

void printST(SymbolTable_t* st) {
    printf(
        "======================================================================"
        "=================\n");
    printf(
        "Name                             Kind       Level       Type          "
        "     Attribute   \n");
    printf(
        "----------------------------------------------------------------------"
        "-----------------\n");
    for (int i = 0; i < st->size; i++)
        printSymbolEntry(st->entries[i], st->level);
    printf(
        "======================================================================"
        "=================\n");
}

void freeST(SymbolTable_t* st) {
    for (int i = 0; i < st->size; i++) freeSymbolEntry(st->entries[i]);
    free(st->entries);
    free(st);
}
