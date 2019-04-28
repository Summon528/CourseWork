#include "symbol_entry.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* KIND_STR[] = {"function", "parameter", "variable",
                                 "constant"};
static const char* TYPE_STR[] = {"int", "float", "double", "bool", "string"};

SymbolEntry_t* newSymbolEntry() {
    SymbolEntry_t* se = (SymbolEntry_t*)malloc(sizeof(SymbolEntry_t));
    se->param_cnt = 0;
    se->arr_size = -1;
    se->param_capacity = INITIAL_CAPACITY;
    se->params = (kind_t*)malloc(sizeof(kind_t) * se->param_capacity);
    return se;
}

void printEntry(SymbolEntry_t* se, int level) {
    char lv_str[16];
    if (level == 0) {
        snprintf(lv_str, sizeof(lv_str), "%d(global)", level);
    } else {
        snprintf(lv_str, sizeof(lv_str), "%d(local)", level);
    }
    if (se->arr_size != -1) {
        char type_str[16];
        snprintf(type_str, sizeof(type_str), "%s[%d]", TYPE_STR[se->type],
                 se->arr_size);
        printf("%-32s %-11s%-12s%-19s", se->name, KIND_STR[se->kind], lv_str,
               type_str);
    } else {
        printf("%-32s %-11s%-12s%-19s", se->name, KIND_STR[se->kind], lv_str,
               TYPE_STR[se->type]);
    }

    if (se->kind == constant) {
        switch (se->type) {
            case _int:
                printf("%d", se->const_ival);
                break;
            case _float:
                printf("%f", se->const_fval);
                break;
            case _double:
                printf("%lf", se->const_dval);
                break;
            case _bool:
                if (se->const_bval) {
                    printf("true");
                } else {
                    printf("false");
                }
                break;
            case _string:
                printf("%s", se->const_sval);
                break;
            default:
                break;
        }
    } else if (se->kind == function) {
        for (int i = 0; i < se->param_cnt; i++) {
            if (i != 0) printf(",");
            printf("%s", KIND_STR[se->params[i]]);
        }
    }
    puts("");
}

void freeSymbolEntry(SymbolEntry_t* se) {
    free(se->params);
    free(se);
}
