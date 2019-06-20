#pragma once
#include "base.h"
#include "literal.h"

void genFun(char *name);
void genFunEnd();
void genGlobal(char *name, Type_t type);
void genLoad(char *name);
void genLiteral(Literal_t *lit, Type_t type);
void genAssign(char *name);
void genPrintStart();
void genPrintEnd(Type_t type);
void genReturn(Type_t type);
void genPromote(Type_t a, Type_t target);
