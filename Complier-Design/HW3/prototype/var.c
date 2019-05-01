#include "var.h"
#include <stdlib.h>
#include "base.h"

Var_t newVar() {
    Var_t* v = malloc(sizeof(Var_t));
    v->arr_capacity = INITIAL_CAPACITY;
    v->arr_cnt = 0;
    return v;
}

void addArr(Var_t* v, int x) {
    if (v->arr_capacity < v->arr_cnt + 1) {
        v->arr_capacity *= 2;
        v->arr = realloc(v->arr, sizeof(int) * v->arr_capacity);
    }
    v->arr[v->arr_cnt++] = x;
}
