#pragma once
#include <stdbool.h>
#include "base.h"

typedef struct literal {
    int ival;
    double dval;
    char sval[256];
    bool bval;
    float fval;
    Type_t type;
} Literal_t;

Literal_t* newLiteral(char* val, Type_t type);
Literal_t* newLiteralCopy(Literal_t* other);
