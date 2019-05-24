#include "base.h"
#include "table_stack.h"
#include "type_struct.h"

void panic(char*, ...);

TypeStruct_t* getType(TableStack_t* ts, char* name, IntArray_t* arr,
                      bool invoke);
TypeStruct_t* checkUMinus(TypeStruct_t* t);
TypeStruct_t* checkULogic(TypeStruct_t* a);
TypeStruct_t* checkLogic(TypeStruct_t* a, TypeStruct_t* b);
TypeStruct_t* checkRelation(TypeStruct_t* a, TypeStruct_t* b);
TypeStruct_t* checkEQNEQ(TypeStruct_t* a, TypeStruct_t* b);
TypeStruct_t* checkArith(TypeStruct_t* a, TypeStruct_t* b);
TypeStruct_t* checkMod(TypeStruct_t* a, TypeStruct_t* b);
TypeStruct_t* checkArraySubscript(TypeStruct_t* a);
void checkAssign(Kind_t k, TypeStruct_t* a, TypeStruct_t* b);
void promoteType1(TypeStruct_t* a, TypeStruct_t* target);
void promoteType2(TypeStruct_t* a, TypeStruct_t* b);
