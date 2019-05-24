#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include "extern.h"

static const char* TYPE_STR[] = {"int",   "float",    "double",
                                 "bool",  "string",   "void",
                                 "array", "function", "unknown"};

void panic(int cnt, ...) {
    va_list valist;
    va_start(valist, cnt);
    printf("##########Error at Line #%d: ", linenum);
    for (int i = 0; i < cnt; i++) {
        if (i != 0) putchar(' ');
        printf("%s", va_arg(valist, char*));
    }
    puts("##########");
    va_end(valist);
}

Type_t getType(TableStack_t* ts, char* name, IntArray_t* arr, bool invoke) {
    SymbolEntry_t* target = findTS(ts, name);
    if (target == NULL) {
        panic(2, "use of undeclared identifier", name);
        return _unknown;
    }
    if (target->kind == function && !invoke) {
        return _function;
    }
    int arr_sig1 = target->arr_sig == NULL ? 0 : target->arr_sig->size;
    int arr_sig2 = arr == NULL ? 0 : arr->size;
    if (arr_sig1 == arr_sig2) {
        return target->type;
    } else if (arr_sig1 > arr_sig2) {
        return _array;
    } else {
        panic(3, "subscripted value is not an array (", name, ")");
        return _unknown;
    }
}

Type_t checkUMinus(Type_t t) {
    switch (t) {
        case _unknown:
            return _unknown;
        case _int:
        case _float:
        case _double:
            return t;
        default:
            panic(3, "invalid argument type", TYPE_STR[t],
                  "to unary minus expression");
            return _unknown;
    }
}

Type_t checkULogic(Type_t a) {
    if (a == _bool) return _bool;
    if (a != _unknown) {
        panic(3, "invalid argument type", TYPE_STR[a],
              "to unary not expression");
    }
    return _unknown;
}

Type_t checkLogic(Type_t a, Type_t b) {
    if (a == _bool && b == _bool) return _bool;
    if (a != _unknown && b != _unknown) {
        panic(5, "invalid operands to logical expression (", TYPE_STR[a], "and",
              TYPE_STR[b], ")");
    }
    return _unknown;
}

Type_t checkRelation(Type_t a, Type_t b) {
    promoteType2(&a, &b);
    if (a != b ||
        !(a == _int || a == _float || a == _double || a == _unknown)) {
        panic(5, "invalid operands to relation expression (", TYPE_STR[a],
              "and", TYPE_STR[b], ")");
        return _unknown;
    }
    return _bool;
}

Type_t checkEQNEQ(Type_t a, Type_t b) {
    promoteType2(&a, &b);
    if (a != b || !(a == _int || a == _float || a == _double || a == _bool ||
                    a == _unknown)) {
        panic(5, "invalid operands to equality expression (", TYPE_STR[a],
              "and", TYPE_STR[b], ")");
        return _unknown;
    }
    return _bool;
}

Type_t checkArith(Type_t a, Type_t b) {
    promoteType2(&a, &b);
    if (a != b ||
        !(a == _int || a == _float || a == _double || a == _unknown)) {
        panic(5, "invalid operands to arithmetic expression (", TYPE_STR[a],
              "and", TYPE_STR[b], ")");
        return _unknown;
    }
    return a;
}

Type_t checkMod(Type_t a, Type_t b) {
    if (a == _int && b == _int) return _int;
    if (a != _unknown && b != _unknown) {
        panic(5, "invalid operands to mod expression (", TYPE_STR[a], "and",
              TYPE_STR[b], ")");
    }
    return _unknown;
}

Type_t checkArraySubscript(Type_t a) {
    if (a == _int) return _int;
    if (a != _unknown) panic(1, "array subscript is not an integer");
    return _unknown;
}

void promoteType2(Type_t* a, Type_t* b) {
    if (*a == *b) return;
    if (*a == _unknown || *b == _unknown) {
        *a = *b = _unknown;
        return;
    }
    if (*a == _double && (*b == _int || *b == _float)) {
        *b = _double;
        return;
    }
    if (*a == _float && *b == _int) {
        *b = _float;
        return;
    }
    if (*b == _double && (*a == _int || *a == _float)) {
        *a = _double;
        return;
    }
    if (*b == _float && *a == _int) {
        *a = _float;
        return;
    }
    return;
}
