#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "util.h"

extern int yyparse();
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc == 1) {
        yyin = stdin;
    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "Open file error\n");
            exit(-1);
        }
        yyin = fp;
    } else {
        fprintf(stderr, "Usage: ./parser [filename]\n");
        exit(0);
    }

    codeout = fopen("code.j", "w");
    fprintf(codeout, ".class public code\n.super java/lang/Object\n\n");
    fprintf(codeout, "; L1\n");

    ts = newTableStack();
    linenum = 1;
    Opt_Symbol = 0;
    in_loop = 0;
    r_state = false;
    label_cnt = 0;
    has_semantic_error = false;
    pushTS(ts);

    yyparse(); /* primary procedure of parser */

    SymbolTable_t *last = getTopTS(ts);
    printST(last);
    for (int i = 0; i < last->size; i++) {
        if (last->entries[i]->kind == function &&
            !last->entries[i]->fun_defed) {
            panic("s s s", "function", last->entries[i]->name,
                  "declared but not defined");
        }
    }

    popTS(ts);
    freeTS(ts);
    if (has_semantic_error) {
        fprintf(stdout, "\n|--------------------------------|\n");
        fprintf(stdout, "|  There is no syntactic error!  |\n");
        fprintf(stdout, "|--------------------------------|\n");
    } else {
        puts(
            "\n|-------------------------------------------|\n"
            "| There is no syntactic and semantic error! |\n"
            "|-------------------------------------------|");
    }
    exit(0);
}
