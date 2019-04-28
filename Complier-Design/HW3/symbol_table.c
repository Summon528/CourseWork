#include "symbol_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable_t* newSymbolTable() {
    SymbolTable_t* st = (SymbolTable_t*)malloc(sizeof(SymbolTable_t));
    st->entry_cnt = 0;
    st->capacity = INITIAL_CAPACITY;
    st->entries =
        (SymbolEntry_t**)malloc(sizeof(SymbolEntry_t*) * st->capacity);
    return st;
}

void printTable(SymbolTable_t* st) {
    printf(
        "======================================================================"
        "=================\n");
    printf(
        "Name                             Kind       Level       Type          "
        "     Attribute   \n");
    printf(
        "----------------------------------------------------------------------"
        "-----------------\n");
    for (int i = 0; i < st->entry_cnt; i++)
        printEntry(st->entries[i], st->level);
    printf(
        "======================================================================"
        "=================\n");
}

SymbolEntry_t* addEntry(SymbolTable_t* st, char* name) {
    if (findFromTable(st, name) != NULL) {
        fprintf(stderr,
                "##########Error at Line #N: %s redeclared.##########\n", name);
        return NULL;
    }
    SymbolEntry_t* se = newSymbolEntry();
    if (st->capacity < st->entry_cnt + 1) {
        st->capacity *= 2;
        st->entries =
            realloc(st->entries, sizeof(SymbolEntry_t*) * st->capacity);
    }
    strcpy(se->name, name);
    st->entries[st->entry_cnt++] = se;
    return se;
}

void addConstEntry(SymbolTable_t* st, char* name, char* val, type_t type) {
    SymbolEntry_t* se = addEntry(st, name);
    if (se == NULL) return;
    se->kind = constant;
    switch (type) {
        case _int:
            sscanf(val, "%d", &se->const_ival);
            break;
        case _float:
            sscanf(val, "%f", &se->const_fval);
            break;
        case _double:
            sscanf(val, "%lf", &se->const_dval);
            break;
        case _bool:
            if (strcmp(val, "true") == 0) {
                se->const_bval = true;
            } else if (strcmp(val, "false") == 0) {
                se->const_bval = false;
            }
            break;
        case _string:
            strncpy(se->const_sval, val, sizeof(se->const_sval));
            break;
        default:
            break;
    }
    se->type = type;
}

void addVarEntry(SymbolTable_t* st, char* name, int arr_size, type_t type) {
    SymbolEntry_t* se = addEntry(st, name);
    if (se == NULL) return;
    se->kind = variable;
    se->arr_size = arr_size;
    se->type = type;
}

void addConstFromList(SymbolTable_t* st, DeclList_t* dl, type_t type) {
    for (int i = 0; i < dl->decl_cnt; i++) {
        addConstEntry(st, dl->entries[i]->name, dl->entries[i]->val, type);
    }
}

void addVarFromList(SymbolTable_t* st, DeclList_t* dl, type_t type) {
    for (int i = 0; i < dl->decl_cnt; i++) {
        addVarEntry(st, dl->entries[i]->name, dl->entries[i]->arr_size, type);
    }
}

void freeSymbolTable(SymbolTable_t* st) {
    for (int i = 0; i < st->entry_cnt; i++) freeSymbolEntry(st->entries[i]);
    free(st);
}

SymbolEntry_t* findFromTable(SymbolTable_t* st, char* name) {
    for (int i = 0; i < st->entry_cnt; i++) {
        if (strcmp(st->entries[i]->name, name) == 0) {
            return st->entries[i];
        }
    }
    return NULL;
}
