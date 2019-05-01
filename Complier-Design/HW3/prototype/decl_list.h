#pragma once
#define INITIAL_CAPACITY 4

typedef struct DeclEntry_t {
    char name[40];
    char val[256];
    int arr_size;
} DeclEntry_t;

typedef struct DeclList_t {
    DeclEntry_t** entries;
    int decl_cnt;
    int capacity;
} DeclList_t;

DeclList_t* newDeclList();
void freeDeclList(DeclList_t* dl);
void addDeclEntry(DeclList_t* dl, char* name, char* val, char* arr_size);
