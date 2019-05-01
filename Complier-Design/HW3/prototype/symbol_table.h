#pragma once
#include <stdbool.h>
#include "decl_list.h"
#include "symbol_entry.h"

typedef struct SymbolTable_t {
    int level;
    SymbolEntry_t** entries;
    int entry_cnt;
    int capacity;
} SymbolTable_t;

SymbolTable_t* newSymbolTable();
void freeSymbolTable(SymbolTable_t* st);

void printTable(SymbolTable_t* st);
void addConstEntry(SymbolTable_t* st, char* name, char* val, type_t type);
SymbolEntry_t* addEntry(SymbolTable_t* st, char* name);
void addVarEntry(SymbolTable_t* st, char* name, int arr_size, type_t type);
void addConstFromList(SymbolTable_t* st, DeclList_t* dl, type_t type);
void addVarFromList(SymbolTable_t* st, DeclList_t* dl, type_t type);
SymbolEntry_t* findFromTable(SymbolTable_t* st, char* name);
