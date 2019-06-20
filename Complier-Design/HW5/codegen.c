#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include "extern.h"
#include "symbol_entry.h"
#include "table_stack.h"

static const char TYPE_DES_CHAR[] = {'I', 'F', 'D', 'Z', '?', '?', '?', '?'};
static const char TYPE_LOWER_CHAR[] = {'i', 'f', 'd', 'i', '?', '?', '?', '?'};

void genFun(char *name) {
    if (strcmp(name, "main") == 0) {
        fprintf(codeout,
                ".method public static main([Ljava/lang/String;)V\n"
                ".limit stack 50\n"
                ".limit locals %d\n",
                MAX_LOCAL);
        in_main_fun = true;
    } else {
        in_main_fun = false;
    }
}

void genFunEnd() { fprintf(codeout, ".end method\n\n"); }

void genGlobal(char *name, Type_t type) {
    fprintf(codeout, ".field public static %s %c\n", name, TYPE_DES_CHAR[type]);
}

void genLoad(char *name) {
    SymbolEntry_t *se = findTS(ts, name);
    if (se == NULL) return;
    if (se->kind == constant) {
        genLiteral(se->const_val, se->type);
    } else if (se->kind == variable && se->var_num == -1) {
        fprintf(codeout, "getstatic code/%s %c\n", se->name,
                TYPE_DES_CHAR[se->type]);
    } else if (se->kind == variable || se->kind == parameter) {
        fprintf(codeout, "%cload %d\n", TYPE_LOWER_CHAR[se->type], se->var_num);
    }
}

void genLiteral(Literal_t *lit, Type_t type) {
    switch (type) {
        case _bool:
            fprintf(codeout, "iconst_%d\n", lit->bval == true);
            break;
        case _string:
            fprintf(codeout, "ldc \"%s\"\n", lit->sval);
            break;
        case _double:
            fprintf(codeout, "ldc2_w %lf\n", lit->dval);
            break;
        case _float:
            fprintf(codeout, "ldc %f\n", lit->fval);
            break;
        case _int:
            fprintf(codeout, "ldc %d\n", lit->ival);
            break;
        default:
            return;
    }
}

void genAssign(char *name) {
    SymbolEntry_t *se = findTS(ts, name);
    if (se == NULL || (se->kind != parameter && se->kind != variable)) return;
    if (se->var_num == -1) {
        fprintf(codeout, "putstatic code/%s %c\n", name,
                TYPE_DES_CHAR[se->type]);
        return;
    }
    fprintf(codeout, "%cstore %d\n", TYPE_LOWER_CHAR[se->type], se->var_num);
}

void genPrintStart() {
    fprintf(codeout, "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
}

void genPrintEnd(Type_t type) {
    if (type == _string) {
        fprintf(
            codeout,
            "invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
        return;
    }
    fprintf(codeout, "invokevirtual java/io/PrintStream/print(%c)V\n",
            TYPE_DES_CHAR[type]);
}

void genReturn(Type_t type) {
    if (in_main_fun || type == _void) {
        fprintf(codeout, "return\n");
    } else {
        fprintf(codeout, "%creturn\n", TYPE_LOWER_CHAR[type]);
    }
}

void genPromote(Type_t a, Type_t target) {
    if (a == target) return;
    if (target == _double && a == _int) {
        fprintf(codeout, "i2d\n");
        return;
    }
    if (target == _double && a == _float) {
        fprintf(codeout, "f2d\n");
        return;
    }
    if (target == _float && a == _int) {
        fprintf(codeout, "i2f\n");
        return;
    }
}
