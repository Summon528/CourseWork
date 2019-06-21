#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
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
        fprintf(codeout, "getstatic output/%s %c\n", se->name,
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
        fprintf(codeout, "putstatic output/%s %c\n", name,
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

char *genPromote0(Type_t a, Type_t target) {
    char *s = (char *)malloc(sizeof(char) * 4);
    memset(s, 0, sizeof(char) * 4);
    if (a == target) return s;
    if (target == _double && a == _int) {
        s = strdup("i2d");
    }
    if (target == _double && a == _float) {
        s = strdup("f2d");
    }
    if (target == _float && a == _int) {
        s = strdup("i2f");
    }
    return s;
}

void genPromote1(Type_t a, Type_t target) {
    char *s = genPromote0(a, target);
    if (s[0] != '\0') fprintf(codeout, "%s\n", s);
    free(s);
}

void genPromote2(Type_t a, Type_t b) {
    genPromote1(b, a);
    char *s = genPromote0(a, b);
    if (s[0] != '\0') {
        fprintf(codeout, "%cstore %d\n", TYPE_LOWER_CHAR[b],
                getTopTS(ts)->next_var);
        fprintf(codeout, "%s\n", s);
        fprintf(codeout, "%cload %d\n", TYPE_LOWER_CHAR[b],
                getTopTS(ts)->next_var);
    }
    free(s);
}

void genArith(Type_t t, char *instr) {
    fprintf(codeout, "%c%s\n", TYPE_LOWER_CHAR[t], instr);
}

void gen(char *instr) { fprintf(codeout, "%s\n", instr); }

void genRelation(Type_t t, char *instr) {
    if (t == _int) {
        fprintf(codeout, "isub\n");
    } else {
        fprintf(codeout, "%ccmpl\n", TYPE_LOWER_CHAR[t]);
    }
    char *label1 = getLabel(), *label2 = getLabel();
    fprintf(codeout,
            "%s %s\n"
            "iconst_0\n"
            "goto %s\n"
            "%s:\n"
            "iconst_1\n"
            "%s:\n",
            instr, label1, label2, label1, label2);
    free(label1);
    free(label2);
}

char *getLabel() {
    char *s = malloc(sizeof(char) * 10);
    sprintf(s, "G%d", label_cnt++);
    return s;
}

char *genLabel(char *label) {
    fprintf(codeout, "%s:\n", label);
    return label;
}
