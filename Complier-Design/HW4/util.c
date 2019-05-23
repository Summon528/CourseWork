#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include "extern.h"

void panic(int cnt, ...) {
    va_list valist;
    va_start(valist, cnt);
    printf("##########Error at Line #%d: ", linenum);
    for (int i = 0; i < cnt; i++) {
        if (i != 0) putchar(' ');
        printf("%s", va_arg(valist, char*));
    }
    puts("##########");
    va_end(valist);
}
