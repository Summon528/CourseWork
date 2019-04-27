#include <stdbool.h>
#include <stdlib.h>

typedef enum { function, parameter, variable, constant } Kind;
typedef enum { _int, _float, _double, _bool, _string } Type;
const char* KIND_STR[] = {"function", "parameter", "variable", "constant"};
const char* TYPE_STR[] = {"int", "float", "double", "bool", "string"};

typedef struct {
    char name[40];
    Kind kind;
    Type type;
    int arr_size;

    int const_ival;
    double const_dval;
    float const_fval;
    char const_sval[256];
    bool const_bval;

    Kind params[100];
    int param_cnt;
} SymbolEntry;

typedef struct {
    int level;
    SymbolEntry* entries[100];
    int entry_cnt;
} SymbolTable;

SymbolEntry* newSymbolEntry();
SymbolTable* newSymbolTable();

void printEntry(SymbolEntry* se, int level);
void printTable(SymbolTable* st);
