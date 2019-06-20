#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include "extern.h"

void genFun(char *name) {
    if (strcmp(name, "main") == 0) {
        fprintf(codeout,
                ".method public static main([Ljava/lang/String;)V\n"
                ".limit stack 50\n");
    }
}

void genFunEnd() { fprintf(codeout, ".end method\n\n"); }
