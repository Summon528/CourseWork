#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include "extern.h"
#include "string.h"
#include "type_struct.h"

static const char* TYPE_STR[] = {"int",    "float", "double",   "bool",
                                 "string", "void",  "function", "unknown"};

static const char* KIND_STR[] = {"function", "parameter", "variable",
                                 "constant"};

void panic(char* fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    printf("##########Error at Line #%d: ", linenum);
    for (char* c = fmt; *c != '\0'; c++) {
        if (*c == ' ') putchar(' ');
        if (*c == 's') printf("%s", va_arg(valist, char*));
        if (*c == 'd') printf("%d", va_arg(valist, int));
        if (*c == 't') {
            TypeStruct_t* tys = va_arg(valist, TypeStruct_t*);
            printf("%s", TYPE_STR[tys->type]);
            if (tys->arr_sig != NULL) {
                for (int i = 0; i < tys->arr_sig->size; i++) {
                    printf("[%d]", tys->arr_sig->arr[i]);
                }
            }
        }
    }
    puts("##########");
    va_end(valist);
}

TypeStruct_t* getType(TableStack_t* ts, char* name, IntArray_t* arr,
                      bool invoke) {
    SymbolEntry_t* target = findTS(ts, name);
    if (target == NULL) {
        panic("s s", "use of undeclared identifier", name);
        return newTypeStruct1(_unknown);
    }
    if (target->kind == function && !invoke) {
        return newTypeStruct1(_function);
    }
    int arr_sig1 = target->arr_sig == NULL ? 0 : target->arr_sig->size;
    int arr_sig2 = arr == NULL ? 0 : arr->size;
    if (arr_sig1 == arr_sig2) {
        return newTypeStruct1(target->type);
    } else if (arr_sig1 > arr_sig2) {
        return newTypeStruct2(target->type, target->arr_sig, arr_sig2);
    } else {
        panic("s s s", "subscripted value is not an array (", name, ")");
        return newTypeStruct1(_unknown);
    }
}

TypeStruct_t* checkUMinus(TypeStruct_t* t) {
    if (eqType1(t, _unknown)) {
        return newTypeStruct1(_unknown);
    } else if (eqType1(t, _int) || eqType1(t, _float) || eqType1(t, _double)) {
        return newTypeStruct1(t->type);
    } else {
        panic("s t s", "invalid argument type", t, "to unary minus expression");
        return newTypeStruct1(_unknown);
    }
}

TypeStruct_t* checkULogic(TypeStruct_t* a) {
    if (eqType1(a, _bool)) return newTypeStruct1(_bool);
    if (!eqType1(a, _unknown)) {
        panic("s t s", "invalid argument type", a, "to unary not expression");
    }
    return newTypeStruct1(_unknown);
}

TypeStruct_t* checkLogic(TypeStruct_t* a, TypeStruct_t* b) {
    if (eqType1(a, _bool) && eqType1(b, _bool)) return newTypeStruct1(_bool);
    if (!eqType1(a, _unknown) && !eqType1(b, _unknown)) {
        panic("s t s t s", "invalid operands to logical expression (", a, "and",
              b, ")");
    }
    return newTypeStruct1(_unknown);
}

TypeStruct_t* checkRelation(TypeStruct_t* a, TypeStruct_t* b) {
    promoteType2(a, b);
    if (!eqType2(a, b) || !(eqType1(a, _int) || eqType1(a, _float) ||
                            eqType1(a, _double) || eqType1(a, _unknown))) {
        panic("s t s t s", "invalid operands to relation expression (", a,
              "and", b, ")");
        return newTypeStruct1(_unknown);
    }
    return newTypeStruct1(_bool);
}

TypeStruct_t* checkEQNEQ(TypeStruct_t* a, TypeStruct_t* b) {
    promoteType2(a, b);
    if (!eqType2(a, b) ||
        !(eqType1(a, _int) || eqType1(a, _float) || eqType1(a, _double) ||
          eqType1(a, _unknown) || eqType1(a, _bool))) {
        panic("s t s t s", "invalid operands to equality expression (", a,
              "and", b, ")");
        return newTypeStruct1(_unknown);
    }
    return newTypeStruct1(_bool);
}

TypeStruct_t* checkArith(TypeStruct_t* a, TypeStruct_t* b) {
    promoteType2(a, b);
    if (!eqType2(a, b) || !(eqType1(a, _int) || eqType1(a, _float) ||
                            eqType1(a, _double) || eqType1(a, _unknown))) {
        panic("s t s t s", "invalid operands to arithmetic expression (", a,
              "and", b, ")");
        return newTypeStruct1(_unknown);
    }
    return newTypeStruct1(a->type);
}

TypeStruct_t* checkMod(TypeStruct_t* a, TypeStruct_t* b) {
    if (eqType1(a, _int) && eqType1(b, _int)) return newTypeStruct1(_int);
    if (!eqType1(a, _unknown) && !eqType1(b, _unknown)) {
        panic("s t s t s", "invalid operands to mod expression (", a, "and", b,
              ")");
    }
    return newTypeStruct1(_unknown);
}

TypeStruct_t* checkArraySubscript(TypeStruct_t* a) {
    if (eqType1(a, _int)) return newTypeStruct1(_int);
    if (!eqType1(a, _unknown))
        panic("s t s", "array subscript is not an integer (", a, ")");
    return newTypeStruct1(_unknown);
}

void checkAssign(Kind_t k, TypeStruct_t* a, TypeStruct_t* b) {
    if (k == constant || k == function) {
        panic("s s", KIND_STR[k], "is not assignable");
        return;
    }
    promoteType1(b, a);
    if (!eqType2(a, b) ||
        !(eqType1(a, _int) || eqType1(a, _float) || eqType1(a, _double) ||
          eqType1(a, _bool) || eqType1(a, _string))) {
        if (eqType1(b, _unknown)) return;
        panic("s t s t s", "invalid operands to assignment expression (", a,
              "and", b, ")");
        return;
    }
    return;
}

TypeStruct_t* checkFunc(char* name, TypeArray_t* ta) {
    SymbolEntry_t* se = findTS(ts, name);
    if (se == NULL) {
        panic("s s", "attempted to invoke undefined function", name);
        return newTypeStruct1(_unknown);
    }
    if (se->kind != function) {
        panic("s s", name, "is not a function");
        return newTypeStruct1(_unknown);
    }
    if (se->params == NULL && ta == NULL) {
        return newTypeStruct1(se->type);
    }
    if ((se->params == NULL && ta != NULL) ||
        (se->params != NULL && ta == NULL) || (se->params->size != ta->size)) {
        panic("s s", "incompatible argument count to function", name);
        return newTypeStruct1(se->type);
    }
    for (int i = 0; i < ta->size; i++) {
        TypeStruct_t* tmp = newTypeStruct2(se->params->arr[i]->type,
                                           se->params->arr[i]->arr_sig, 0);
        promoteType1(ta->arr[i], tmp);
        if (!eqType2(tmp, ta->arr[i])) {
            if (!eqType1(ta->arr[i], _unknown)) {
                panic("s s s d s t s t s", "incompatible argument to function",
                      name, "at position", i + 1, "(", tmp, "and", ta->arr[i],
                      ")");
            }
        }
        freeTypeStruct(tmp);
    }
    return newTypeStruct1(se->type);
}

void promoteType1(TypeStruct_t* a, TypeStruct_t* target) {
    if (eqType2(a, target)) return;
    if (eqIntArray(a->arr_sig, target->arr_sig)) {
        if (target->type == _unknown || a->type == _unknown) {
            a->type = _unknown;
            return;
        }
        if (target->type == _double && (a->type == _int || a->type == _float)) {
            a->type = _double;
            return;
        }
        if (target->type == _float && a->type == _int) {
            a->type = _float;
            return;
        }
    }
}

void promoteType2(TypeStruct_t* a, TypeStruct_t* b) {
    promoteType1(a, b);
    promoteType1(b, a);
}
