#include "decl_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DeclList_t* newDeclList() {
    DeclList_t* dl = malloc(sizeof(DeclList_t));
    dl->decl_cnt = 0;
    dl->capacity = INITIAL_CAPACITY;
    dl->entries = (DeclEntry_t**)malloc(sizeof(DeclEntry_t*) * dl->capacity);
    return dl;
}

void addDeclEntry(DeclList_t* dl, char* name, char* val, char* arr_size) {
    if (dl->capacity < dl->decl_cnt + 1) {
        dl->capacity *= 2;
        dl->entries = realloc(dl->entries, sizeof(DeclEntry_t*) * dl->capacity);
    }
    DeclEntry_t* de = (DeclEntry_t*)malloc(sizeof(DeclEntry_t));
    strncpy(de->name, name, sizeof(de->name));
    if (val != NULL) strncpy(de->val, val, sizeof(de->val));
    if (arr_size != NULL) {
        sscanf(arr_size, "%d", &de->arr_size);
    } else {
        de->arr_size = -1;
    }
    dl->entries[dl->decl_cnt++] = de;
}

void freeDeclList(DeclList_t* dl) {
    free(dl->entries);
    free(dl);
}
