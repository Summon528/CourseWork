%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%token WHILE DO IF ELSE TRUE FALSE FOR INT PRINT CONST READ BOOLEAN
%token BOOL VOID FLOAT DOUBLE STRING CONTINUE BREAK RETURN
%token SCI_NUM INT_NUM FLOAT_NUM ID

%right '='
%left OR
%left AND
%right '!'
%left REL_OP
%left '+' '-'
%left '*' '/' '%'
%right NEG
%%

program : decl_list function_def decl_and_def_list
        ;

decl_list : /* empty */
          | decl_list declaration
          | decl_list function_decl
          ;

decl_and_def_list : /* empty */
                  | decl_and_def_list declaration
                  | decl_and_def_list function_decl
                  | decl_and_def_list function_def
                  ;

function_decl : type ID '(' formal_arg_list ')' ';'
              | VOID ID '(' formal_arg_list ')' ';'
              ;

function_def : type ID '(' formal_arg_list ')' compound_stat
             | VOID ID '(' formal_arg_list ')' compound_stat
             ;

formal_arg_list : /* empty */
                | non_empty_formal_arg_list
                ;

non_empty_formal_arg_list : non_empty_formal_arg_list ',' formal_arg
                          | formal_arg
                          ;

formal_arg : type ID
           | type ID brackets_int
           ;

state_decl_list : state_decl_list declaration
                | state_decl_list statement
                | statement
                | declaration
                ;

declaration : type var_decl_list ';'
            | CONST type const_decl_list ';'
            ;

const_decl_list : const_decl_list ',' ID '=' literal
                | ID '=' literal
                ;

var_decl_list : var_decl_list ',' var_decl
              | var_decl
              ;

var_decl : ID
         | ID '=' expr
         | ID brackets_int
         | ID brackets_int '=' '{' expr_list '}'
         ;

brackets_int : '[' INT_NUM ']'
             | brackets_int '[' INT_NUM ']'
             ;

statement : compound_stat
          | simple_stat
          | if_stat
          | while_stat
          | for_stat
          | jump_stat
          | func_invoke ';'
          ;

compound_stat : '{' state_decl_list '}'
              | '{' '}'
              ;

simple_stat : var_ref '=' expr ';' 
            | PRINT expr ';'
            | READ var_ref ';'
            ;

if_stat : IF '(' expr ')' compound_stat
        | IF '(' expr ')' compound_stat ELSE compound_stat
        ;

while_stat : WHILE '(' expr ')' compound_stat
           | DO compound_stat WHILE '(' expr ')' ';'
           ;

for_stat : FOR '(' for_expr ';' for_expr ';' for_expr ')' compound_stat
         ;

for_expr : non_empty_for_expr
         | /* empty */
         ;

non_empty_for_expr : var_ref '=' expr
                   | expr
                   | non_empty_for_expr ','  expr
                   | non_empty_for_expr ',' var_ref '=' expr
                   ;

jump_stat : RETURN expr ';'
          | BREAK ';'
          | CONTINUE ';'
          ;

func_invoke : ID '(' expr_list ')'
            ;


expr_list : non_empty_expr_list
          | /* empty */
          ;

non_empty_expr_list : expr_list ',' expr
          | expr
          ;

expr : expr AND expr_lv4
     | expr OR expr_lv4
     | expr_lv4
     ;

expr_lv4 : '!' expr_lv4
         | expr_lv3
         ;

expr_lv3 : expr_lv3 '*' expr_lv2
         | expr_lv3 '/' expr_lv2
         | expr_lv3 '%' expr_lv2
         | expr_lv3 '+' expr_lv2
         | expr_lv3 '-' expr_lv2
         | expr_lv3 REL_OP expr_lv2
         | expr_lv2
         ;

expr_lv2 : expr_lv1
         | '-' expr_lv2
         ;

expr_lv1 : func_invoke
         | literal
         | var_ref
         | '(' expr ')'
         ;

var_ref : ID brackets_expr
        | ID
        ;

brackets_expr : '[' expr ']'
              | brackets_expr '[' expr ']'
              ;

literal : INT_NUM
        | SCI_NUM
        | FLOAT_NUM
        | TRUE
        | FALSE
        | STRING
        ;

type : INT
     | FLOAT
     | DOUBLE
     | STRING
     | BOOL
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
}

int  main( int argc, char **argv )
{
    if( argc != 2 ) {
        fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
        exit(0);
    }

    FILE *fp = fopen( argv[1], "r" );
    
    if( fp == NULL )  {
        fprintf( stdout, "Open  file  error\n" );
        exit(-1);
    }
    
    yyin = fp;
    yyparse();

    fprintf( stdout, "\n" );
    fprintf( stdout, "|--------------------------------|\n" );
    fprintf( stdout, "|  There is no syntactic error!  |\n" );
    fprintf( stdout, "|--------------------------------|\n" );
    exit(0);
}
