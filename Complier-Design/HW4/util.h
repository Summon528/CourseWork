#include "base.h"
#include "table_stack.h"
#include "type_array.h"
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
TypeStruct_t* checkFunc(char* name, TypeArray_t* ta);
void checkArrayInit(char* name, Type_t type, IntArray_t* dim,
                    TypeArray_t* init);
void checkAssign(Kind_t k, TypeStruct_t* a, TypeStruct_t* b);
void checkReturn(TypeStruct_t* a, Type_t target);
void checkIO(TypeStruct_t* a);
void checkCondition(TypeStruct_t* a);
void checkInLoop(int in_loop);
void promoteType1(TypeStruct_t* a, TypeStruct_t* target);
void promoteType2(TypeStruct_t* a, TypeStruct_t* b);
