#include "symbol_entry.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* KIND_STR[] = {"function", "parameter", "variable",
                                 "constant"};
static const char* TYPE_STR[] = {"int",  "float",  "double",
                                 "bool", "string", "void"};

SymbolEntry_t* newSymbolEntry() {
    SymbolEntry_t* se = (SymbolEntry_t*)malloc(sizeof(SymbolEntry_t));
    se->arr_sig = NULL;
    se->const_val = NULL;
    se->params = NULL;
    memset(se->name, 0, sizeof(se->name));
    return se;
}

void printSymbolEntry(SymbolEntry_t* se, int level) {
    char lv_str[16];
    if (level == 0) {
        snprintf(lv_str, sizeof(lv_str) - 1, "%d(global)", level);
    } else {
        snprintf(lv_str, sizeof(lv_str) - 1, "%d(local)", level);
    }
    if (se->arr_sig != NULL) {
        char type_str[256];
        int rbuf = 256 - 1;
        if (se->kind == constant && se->const_val != NULL) {
            strncpy(type_str, TYPE_STR[se->const_val->type], rbuf);
        } else {
            strncpy(type_str, TYPE_STR[se->type], rbuf);
        }
        rbuf -= strlen(TYPE_STR[se->type]);

        for (int i = 0; i < se->arr_sig->size; i++) {
            char tmp[64];
            snprintf(tmp, sizeof(tmp), "[%d]", se->arr_sig->arr[i]);
            strncat(type_str, tmp, rbuf);
            rbuf -= strlen(tmp);
        }

        printf("%-32s %-11s%-12s%-19s", se->name, KIND_STR[se->kind], lv_str,
               type_str);
    } else {
        if (se->kind == constant && se->const_val != NULL) {
            printf("%-32s %-11s%-12s%-19s", se->name, KIND_STR[se->kind],
                   lv_str, TYPE_STR[se->const_val->type]);
        } else {
            printf("%-32s %-11s%-12s%-19s", se->name, KIND_STR[se->kind],
                   lv_str, TYPE_STR[se->type]);
        }
    }

    if (se->kind == constant) {
        switch (se->type) {
            case _int:
                printf("%d", se->const_val->ival);
                break;
            case _float:
                printf("%f", se->const_val->fval);
                break;
            case _double:
                printf("%lf", se->const_val->dval);
                break;
            case _bool:
                if (se->const_val->bval) {
                    printf("true");
                } else {
                    printf("false");
                }
                break;
            case _string:
                printf("%s", se->const_val->sval);
                break;
            default:
                break;
        }
    } else if (se->kind == function && se->params != NULL) {
        for (int i = 0; i < se->params->size; i++) {
            if (i != 0) printf(",");
            printf("%s", TYPE_STR[se->params->arr[i]->type]);
            IntArray_t* arr_sig = se->params->arr[i]->arr_sig;
            if (arr_sig != NULL) {
                for (int j = 0; j < arr_sig->size; j++) {
                    printf("[%d]", arr_sig->arr[j]);
                }
            }
        }
    }
    puts("");
}

void freeSymbolEntry(SymbolEntry_t* se) {
    freeIntArray(se->arr_sig);
    freeDeclArray(se->params);
    free(se);
}
