#include <stdio.h>
#include <stdlib.h>
#include "extern.h"

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

    ts = newTableStack();
    linenum = 1;
    pushTS(ts);

    yyparse(); /* primary procedure of parser */

    printST(getTopTS(ts));
    popTS(ts);
    freeTS(ts);

    fprintf(stdout, "\n|--------------------------------|\n");
    fprintf(stdout, "|  There is no syntactic error!  |\n");
    fprintf(stdout, "|--------------------------------|\n");
    exit(0);
}
