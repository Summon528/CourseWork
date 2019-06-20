%{
#include "everything.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

extern int linenum;
extern FILE *yyin;
extern char *yytext;
extern char buf[256];
extern TableStack_t* ts;
extern Type_t cur_type;
extern Type_t cur_fun_type;
extern int in_loop;
extern bool r_state;

int yylex();
int yyerror( char *msg );

%}

%token<text>  ID
%token<literal>  INT_CONST
%token<literal>  FLOAT_CONST
%token<literal>  SCIENTIFIC
%token<literal>  STR_CONST

%token  LE_OP
%token  NE_OP
%token  GE_OP
%token  EQ_OP
%token  AND_OP
%token  OR_OP

%token  READ
%token  BOOLEAN
%token  WHILE
%token  DO
%token  IF
%token  ELSE
%token<literal>  TRUE
%token<literal>  FALSE
%token  FOR
%token<type>  INT
%token  PRINT
%token<type>  BOOL
%token  VOID
%token<type>  FLOAT
%token<type>  DOUBLE
%token<type>  STRING
%token  CONTINUE
%token  BREAK
%token  RETURN
%token  CONST

%token  L_PAREN
%token  R_PAREN
%token  COMMA
%token  SEMICOLON
%token  ML_BRACE
%token  MR_BRACE
%token  L_BRACE
%token  R_BRACE
%token  ADD_OP
%token  SUB_OP
%token  MUL_OP
%token  DIV_OP
%token  MOD_OP
%token  ASSIGN_OP
%token  LT_OP
%token  GT_OP
%token  NOT_OP

/*  Program 
    Function 
    Array 
    Const 
    IF 
    ELSE 
    RETURN 
    FOR 
    WHILE
*/

%union {
    char* text;
    Literal_t* literal;
    IntArray_t* iarray;
    ParamItem_t* param_item;
    ParamArray_t* param_array;
    Type_t type;
    TypeStruct_t* type_struct;
    TypeArray_t* type_array;
}

%type<iarray> dim dimension;
%type<literal> literal_const;
%type<param_array> parameter_list;
%type<type_struct> element logical_expression variable_reference logical_term;
%type<type_struct> logical_factor relation_expression arithmetic_expression;
%type<type_struct> term factor sign_literal_const function_invoke control_expression;
%type<type_array> logical_expression_list literal_list initial_array;
%type<type> scalar_type;
%start program
%%

program : decl_list funct_def decl_and_def_list 
        ;

decl_list : decl_list var_decl
          | decl_list const_decl
          | decl_list funct_decl
          |
          ;


decl_and_def_list : decl_and_def_list var_decl
                  | decl_and_def_list const_decl
                  | decl_and_def_list funct_decl
                  | decl_and_def_list funct_def
                  | 
                  ;

funct_def : scalar_type ID L_PAREN R_PAREN {
                pushSTFunc(getTopTS(ts), $2, $1, NULL, 0);
                TypeStruct_t* tmp = newTypeStruct1($1);
                cur_fun_type = tmp->type;
                freeTypeStruct(tmp);
            }
            compound_statement { if(!r_state) panic("s", "last statement in function is not a return statement"); }
          | scalar_type ID L_PAREN parameter_list R_PAREN {
                pushSTFunc(getTopTS(ts), $2, $1, $4, 0);
                TypeStruct_t* tmp = newTypeStruct1($1);
                cur_fun_type = tmp->type;
                freeTypeStruct(tmp);
            } L_BRACE {
                pushTS(ts); 
                pushSTParamArray(getTopTS(ts), $4);
            }
            var_const_stmt_list R_BRACE {
                printST(getTopTS(ts));
                popTS(ts);
                freeParamArray($4);
                if(!r_state) panic("s", "last statement in function is not a return statement");
            } 
            | VOID ID L_PAREN R_PAREN {
                pushSTFunc(getTopTS(ts), $2, _void, NULL, 0);
                TypeStruct_t* tmp = newTypeStruct1(_void);
                cur_fun_type = tmp->type;
                freeTypeStruct(tmp);
            }
            compound_statement 
          | VOID ID L_PAREN parameter_list R_PAREN {
                pushSTFunc(getTopTS(ts), $2, _void, $4, 0);
                TypeStruct_t* tmp = newTypeStruct1(_void);
                cur_fun_type = tmp->type;
                freeTypeStruct(tmp);
            } L_BRACE {
                pushTS(ts);
                pushSTParamArray(getTopTS(ts), $4);
            }
            var_const_stmt_list R_BRACE {
                printST(getTopTS(ts));
                popTS(ts);
                freeParamArray($4);
            }
          ;

funct_decl : scalar_type ID L_PAREN R_PAREN SEMICOLON { pushSTFunc(getTopTS(ts), $2, $1, NULL, 1); }
           | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON { pushSTFunc(getTopTS(ts), $2, $1, $4, 1), freeParamArray($4); }
           | VOID ID L_PAREN R_PAREN SEMICOLON { pushSTFunc(getTopTS(ts), $2, _void, NULL, 1); }
           | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON { pushSTFunc(getTopTS(ts), $2, _void, $4, 1), freeParamArray($4); }
           ;

parameter_list : parameter_list COMMA scalar_type ID { $$ = pushParamArray($1, newParamItem($4, $3, NULL)); }
               | parameter_list COMMA scalar_type ID dim { $$ = pushParamArray($1, newParamItem($4, $3, $5)); }
               | scalar_type ID dim { $$ = pushParamArray(newParamArray(), newParamItem($2, $1, $3)); }
               | scalar_type ID { $$ = pushParamArray(newParamArray(), newParamItem($2, $1, NULL)); }
               ;

var_decl : scalar_type identifier_list SEMICOLON

identifier_list : identifier_list COMMA ID { pushSTVar(getTopTS(ts), $3, NULL); }
                | identifier_list COMMA ID ASSIGN_OP logical_expression {
                    TypeStruct_t* tmp = newTypeStruct1(cur_type);
                    checkAssign(variable, tmp, $5);
                    pushSTVar(getTopTS(ts), $3, NULL);
                    freeTypeStruct(tmp);
                    freeTypeStruct($5);
                }
                | identifier_list COMMA ID dim ASSIGN_OP initial_array {
                    checkArrayInit($3, cur_type, $4, $6);
                    pushSTVar(getTopTS(ts), $3, $4);
                    freeTypeArray($6);
                }
                | identifier_list COMMA ID dim {
                    checkArrayInit($3, cur_type, $4, NULL);
                    pushSTVar(getTopTS(ts), $3, $4);
                }
                | ID dim ASSIGN_OP initial_array {
                    checkArrayInit($1, cur_type, $2, $4);
                    pushSTVar(getTopTS(ts), $1, $2);
                    freeTypeArray($4);
                }
                | ID dim {
                    checkArrayInit($1, cur_type, $2, NULL);
                    pushSTVar(getTopTS(ts), $1, $2);
                }
                | ID ASSIGN_OP logical_expression {
                    TypeStruct_t* tmp = newTypeStruct1(cur_type);
                    checkAssign(variable, tmp, $3);
                    pushSTVar(getTopTS(ts), $1, NULL);
                    freeTypeStruct(tmp);
                    freeTypeStruct($3);
                }
                | ID { pushSTVar(getTopTS(ts), $1, NULL); }
                ;
                
initial_array : L_BRACE literal_list R_BRACE { $$ = $2; }
              ;

literal_list : literal_list COMMA logical_expression { $$ = pushTypeArray($1, $3); }
             | logical_expression { $$ = pushTypeArray(newTypeArray(), $1); }
             |  { $$ = newTypeArray(); }
             ;

const_decl : CONST scalar_type const_list SEMICOLON

const_list : const_list COMMA ID ASSIGN_OP literal_const {
                TypeStruct_t* tmp1 = newTypeStruct1(cur_type);
                TypeStruct_t* tmp2 = newTypeStruct1($5->type);
                checkAssign(variable, tmp1, tmp2);
                pushSTConst(getTopTS(ts), $3, $5);
                freeTypeStruct(tmp1);
                freeTypeStruct(tmp2);
            }
           | ID ASSIGN_OP literal_const { 
                TypeStruct_t* tmp1 = newTypeStruct1(cur_type);
                TypeStruct_t* tmp2 = newTypeStruct1($3->type);
                checkAssign(variable, tmp1, tmp2);
                pushSTConst(getTopTS(ts), $1, $3); 
                freeTypeStruct(tmp1);
                freeTypeStruct(tmp2);
            }
           ;

dim : dim ML_BRACE INT_CONST MR_BRACE { $$ = pushIntArray($1, $3->ival); }
    | ML_BRACE INT_CONST MR_BRACE { $$ = pushIntArray(newIntArray(), $2->ival); }
    ;

compound_statement : L_BRACE { pushTS(ts); } var_const_stmt_list R_BRACE { printST(getTopTS(ts)); popTS(ts);}
                   ;

var_const_stmt_list : var_const_stmt_list statement
                    | var_const_stmt_list var_decl { r_state = false; }
                    | var_const_stmt_list const_decl { r_state = false; }
                    | { r_state = false; }
                    ;

statement : compound_statement { r_state = false; }
          | simple_statement { r_state = false; }
          | conditional_statement { r_state = false; }
          | while_statement { r_state = false; }
          | for_statement { r_state = false; }
          | function_invoke_statement { r_state = false; }
          | jump_statement { r_state = false; }
          | return_statement { r_state = true; }
          ;     

simple_statement : var_assign SEMICOLON
                 | PRINT logical_expression SEMICOLON { checkIO($2); freeTypeStruct($2); }
                 | READ variable_reference SEMICOLON { checkIO($2); freeTypeStruct($2); }
                 ;

conditional_statement : _conditional_statement compound_statement
                      | _conditional_statement compound_statement ELSE compound_statement
                      ;

_conditional_statement : IF L_PAREN logical_expression R_PAREN  {
                            checkCondition($3);
                            freeTypeStruct($3);
                        };

while_statement : WHILE L_PAREN logical_expression R_PAREN {
                        checkCondition($3);
                        freeTypeStruct($3);
                        in_loop++; 
                    } compound_statement { in_loop--; }
                | DO { in_loop++; } compound_statement { in_loop--; } WHILE L_PAREN logical_expression R_PAREN SEMICOLON {
                    checkCondition($7);
                    freeTypeStruct($7);
                }
                ;

for_statement : FOR L_PAREN initial_expression_list SEMICOLON control_expression SEMICOLON increment_expression_list R_PAREN {
                    checkCondition($5);
                    freeTypeStruct($5); 
                    in_loop++;
                }
                    compound_statement {
                        in_loop--;
                    }
              ;

initial_expression_list : initial_expression
                        |
                        ;

initial_expression : initial_expression COMMA var_assign
                   | initial_expression COMMA logical_expression
                   | logical_expression
                   | var_assign


control_expression : logical_expression
                   | { $$ = newTypeStruct1(_bool); }
                   ;

increment_expression_list : increment_expression 
                          |
                          ;

increment_expression : increment_expression COMMA var_assign
                     | increment_expression COMMA logical_expression
                     | logical_expression
                     | var_assign
                     ;

var_assign : ID ASSIGN_OP logical_expression { 
                TypeStruct_t* tmp = getType(ts, $1, NULL, false);
                if(!eqType1(tmp, _unknown)) checkAssign(findTS(ts, $1)->kind, tmp, $3); 
                freeTypeStruct(tmp);
                freeTypeStruct($3);
           }
           | ID dimension ASSIGN_OP logical_expression {
                TypeStruct_t* tmp = getType(ts, $1, $2, false);
                if(!eqType1(tmp, _unknown)) checkAssign(findTS(ts, $1)->kind, tmp, $4); 
                freeTypeStruct(tmp);
                freeTypeStruct($4);
            }
           ;

function_invoke_statement : function_invoke SEMICOLON
                          ;

jump_statement : CONTINUE SEMICOLON { checkInLoop(in_loop); }
               | BREAK SEMICOLON { checkInLoop(in_loop); }
               ;

return_statement : RETURN logical_expression SEMICOLON { checkReturn($2, cur_fun_type); freeTypeStruct($2); }
                ; 

variable_reference : ID dimension { $$ = getType(ts, $1, $2, false); }
                   | ID { $$ = getType(ts, $1, NULL, false); }
                   ;


logical_expression : logical_expression OR_OP logical_term { $$ = checkLogic($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                   | logical_term
                   ;

logical_term : logical_term AND_OP logical_factor { $$ = checkLogic($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
             | logical_factor
             ;

logical_factor : NOT_OP logical_factor { $$ = checkULogic($2), freeTypeStruct($2); }
               | relation_expression
               ;

relation_expression : relation_expression relation_operator arithmetic_expression { $$ = checkRelation($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                    | relation_expression NE_OP arithmetic_expression { $$ = checkEQNEQ($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                    | relation_expression EQ_OP arithmetic_expression { $$ = checkEQNEQ($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                    | arithmetic_expression
                    ;

relation_operator : LT_OP
                  | LE_OP
                  | GE_OP
                  | GT_OP
                  ;

arithmetic_expression : arithmetic_expression ADD_OP term { $$ = checkArith($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                      | arithmetic_expression SUB_OP term { $$ = checkArith($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
                      | term
                      ;

term : term MUL_OP factor { $$ = checkArith($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
     | term DIV_OP factor { $$ = checkArith($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
     | term MOD_OP factor { $$ = checkMod($1, $3), freeTypeStruct($1), freeTypeStruct($3); }
     | factor
     ;

factor : sign_literal_const
       | element
       ;

element : SUB_OP element { $$ = checkUMinus($2), freeTypeStruct($2); }
        | variable_reference
        | L_PAREN logical_expression R_PAREN { $$ = $2; }
        | function_invoke
        ;

function_invoke : ID L_PAREN logical_expression_list R_PAREN { $$ = checkFunc($1, $3), freeTypeArray($3); }
                | ID L_PAREN R_PAREN { $$ = checkFunc($1, NULL); }
                ;

logical_expression_list : logical_expression_list COMMA logical_expression { $$ = pushTypeArray($1, $3); }
                        | logical_expression { $$ = pushTypeArray(newTypeArray(), $1); }
                        ;

dimension : dimension ML_BRACE logical_expression MR_BRACE { checkArraySubscript($3), freeTypeStruct($3), $$ = pushIntArray($1, 0); }
          | ML_BRACE logical_expression MR_BRACE { checkArraySubscript($2), freeTypeStruct($2), $$ = pushIntArray(newIntArray(), 0); }
          ;



scalar_type : INT { $$ = cur_type = $1; }
            | DOUBLE { $$ = cur_type = $1; }
            | STRING { $$ = cur_type = $1; }
            | BOOL { $$ = cur_type = $1; }
            | FLOAT { $$ = cur_type = $1; }
            ;

sign_literal_const : SUB_OP sign_literal_const { $$ = checkUMinus($2), freeTypeStruct($2); }
                   | literal_const { $$ = newTypeStruct1($1->type); }
                   ;

literal_const : INT_CONST
              | FLOAT_CONST
              | SCIENTIFIC
              | STR_CONST
              | TRUE
              | FALSE
              ;


%%

int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
    fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
    fprintf( stderr, "|\n" );
    fprintf( stderr, "| Unmatched token: %s\n", yytext );
    fprintf( stderr, "|--------------------------------------------------------------------------\n" );
    exit(-1);
    //  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}
