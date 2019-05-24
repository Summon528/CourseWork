#include "base.h"
#include "table_stack.h"

void panic(int cnt, ...);

Type_t getType(TableStack_t* ts, char* name, IntArray_t* arr, bool invoke);
Type_t checkUMinus(Type_t t);
Type_t checkULogic(Type_t a);
Type_t checkLogic(Type_t a, Type_t b);
Type_t checkRelation(Type_t a, Type_t b);
Type_t checkEQNEQ(Type_t a, Type_t b);
Type_t checkArith(Type_t a, Type_t b);
Type_t checkMod(Type_t a, Type_t b);
Type_t checkArraySubscript(Type_t a);
void checkAssign(Kind_t k, Type_t a, Type_t b);
void promoteType1(Type_t* a, Type_t target);
void promoteType2(Type_t* a, Type_t* b);
