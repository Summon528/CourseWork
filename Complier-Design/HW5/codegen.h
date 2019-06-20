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
char *genPromote0(Type_t a, Type_t target);
void genPromote1(Type_t a, Type_t target);
void genPromote2(Type_t a, Type_t b);
void genArith(Type_t t, char *instr);
void genRelation(Type_t t, char *instr);
void gen(char *instr);
char *genLabel();
