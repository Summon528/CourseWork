#include "literal.h"
#include <stdlib.h>
#include <string.h>

Literal_t* newLiteral(char* val, Type_t type) {
    Literal_t* l = malloc(sizeof(Literal_t));
    switch (type) {
        case _int:
            l->ival = atoi(val);
            break;
        case _bool:
            if (strcmp(val, "true") == 0) {
                l->bval = true;
            } else if (strcmp(val, "false") == 0) {
                l->bval = false;
            }
        case _double:
        case _float:
            l->dval = atof(val);
            l->fval = atof(val);
            break;
        case _string:
            strncpy(l->sval, val, sizeof(l->sval) - 1);
            break;
    }
    return l;
}

Literal_t* newLiteralCopy(Literal_t* other) {
    if (other == NULL) return NULL;
    Literal_t* l = malloc(sizeof(Literal_t));
    memcpy(l, other, sizeof(Literal_t));
    return l;
}
