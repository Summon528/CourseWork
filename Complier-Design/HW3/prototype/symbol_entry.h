#pragma once
#include <stdbool.h>
#define INITIAL_CAPACITY 4

typedef enum kind_t { function, parameter, variable, constant } kind_t;
typedef enum type_t { _int, _float, _double, _bool, _string } type_t;

typedef struct SymbolEntry_t {
    char name[40];
    kind_t kind;
    type_t type;
    int arr_size;

    int const_ival;
    double const_dval;
    float const_fval;
    char const_sval[256];
    bool const_bval;

    kind_t* params;
    int param_cnt;
    int param_capacity;
} SymbolEntry_t;

SymbolEntry_t* newSymbolEntry();
void freeSymbolEntry(SymbolEntry_t* se);

void printEntry(SymbolEntry_t* se, int level);
