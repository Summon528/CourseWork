#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"
#include "util.h"

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

void pushSTVar(SymbolTable_t* st, char* name, IntArray_t* arr) {
    SymbolEntry_t* se = pushST(st, name);
    if (se == NULL) {
        panic("s s", name, "redeclared");
        return;
    }
    se->kind = variable;
    se->const_val = NULL;
    se->arr_sig = arr;
    se->type = cur_type;
    se->params = NULL;
}

void pushSTConst(SymbolTable_t* st, char* name, Literal_t* lit) {
    SymbolEntry_t* se = pushST(st, name);
    if (se == NULL) {
        panic("s s", name, "redeclared");
        return;
    }
    se->kind = constant;
    se->const_val = lit;
    se->arr_sig = NULL;
    se->type = cur_type;
    se->params = NULL;
}

void pushSTParamArray(SymbolTable_t* st, ParamArray_t* da) {
    for (int i = 0; i < da->size; i++) {
        SymbolEntry_t* se = pushST(st, da->arr[i]->name);
        if (se == NULL) {
            panic("s s", da->arr[i]->name, "redeclared");
            continue;
        }
        se->kind = parameter;
        se->arr_sig = newIntArrayCpy(da->arr[i]->arr_sig);
        se->type = da->arr[i]->type;
        se->params = NULL;
    }
}

void pushSTFunc(SymbolTable_t* st, char* name, Type_t type, ParamArray_t* da,
                int decl) {
    SymbolEntry_t* se = findST(st, name);
    if (se != NULL) {
        if (se->kind != function || decl) {
            panic("s s", name, "redeclared");
            return;
        }
        if (se->fun_defed) {
            panic("s s", "redefinition of", name);
            return;
        }
        if (!eqParamArray(se->params, da)) {
            panic("s s", "conflicting types for", name);
            return;
        }
        se->fun_defed = true;
        return;
    }
    se = pushST(st, name);
    se->kind = function;
    se->const_val = NULL;
    se->arr_sig = NULL;
    se->type = type;
    se->params = copyParamArray(da);
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
    if (!Opt_Symbol || st->size == 0) return;
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
