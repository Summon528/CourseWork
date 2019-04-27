#include "symbol_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

SymbolEntry* newSymbolEntry() {
    SymbolEntry* se = (SymbolEntry*)malloc(sizeof(SymbolEntry));
    se->param_cnt = 0;
    se->arr_size = -1;
    return se;
}

SymbolTable* newSymbolTable() {
    SymbolTable* st = (SymbolTable*)malloc(sizeof(SymbolTable));
    st->entry_cnt = 0;
    return st;
}

void printEntry(SymbolEntry* se, int level) {
    char attr_str[256] = {};
    if (se->kind == constant) {
        switch (se->type) {
            case _int:
                sprintf(attr_str, "%d", se->const_ival);
                break;
            case _float:
                sprintf(attr_str, "%f", se->const_fval);
                break;
            case _double:
                sprintf(attr_str, "%lf", se->const_dval);
                break;
            case _bool:
                if (se->const_bval) {
                    strncpy(attr_str, "true", 5);
                } else {
                    strncpy(attr_str, "false", 6);
                }
                break;
            case _string:
                strncpy(attr_str, se->const_sval, 255);
                break;
            default:
                break;
        }
    } else if (se->kind == function) {
        for (int i = 0; i < se->param_cnt; i++) {
            if (i != 0) strcat(attr_str, ",");
            strcat(attr_str, KIND_STR[se->params[i]]);
        }
    } else {
        attr_str[0] = '\0';
    }
    char lv_str[16];
    sprintf(lv_str, "%d", level);
    printf("%-32s %-11s%-12s%-19s%s\n", se->name, KIND_STR[se->kind], lv_str,
           TYPE_STR[se->type], attr_str);
}

void printTable(SymbolTable* st) {
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

void add_consti_entry(SymbolTable* st, char* name, char* val) {
    SymbolEntry* se = newSymbolEntry();
    strcpy(se->name, name);
    se->kind = constant;
    sscanf(val, "%d", &se->const_ival);
    st->entries[st->entry_cnt++] = se;
}

void addConstEntry(SymbolTable* st, char* name, char* val, Type type) {
    SymbolEntry* se = newSymbolEntry();
    strcpy(se->name, name);
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
            strncpy(se->const_sval, val, 255);
            break;
        default:
            break;
    }
    se->type = type;
    st->entries[st->entry_cnt++] = se;
}

int main() {
    SymbolTable* st = newSymbolTable();
    addConstEntry(st, "abc", "123", _int);
    addConstEntry(st, "cfg", "6866123", _int);
    addConstEntry(st, "regex", "68", _int);
    addConstEntry(st, "fl", "68.3", _float);
    addConstEntry(st, "sci", "1e3", _double);
    addConstEntry(st, "d", "1.3", _double);
    addConstEntry(st, "b", "1.3", _bool);
    addConstEntry(st, "hahas", "tsengcy", _string);
    printTable(st);
}
