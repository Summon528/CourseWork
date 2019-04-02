#include <stdio.h>
#include <string.h>
#define MAX_D 1000005
#define CLEAR(x) memset((x), '0', sizeof((x)))

char a[MAX_D], b[MAX_D], rb[MAX_D];

void add() {
    int c = 0;
    for (int i = 0; i < MAX_D; i++) {
        a[i] += rb[i] - '0' + c;
        c = a[i] > '9';
        a[i] -= c * 10;
    }
    CLEAR(b), CLEAR(rb);
}

void print() {
    int i = MAX_D;
    while (a[--i] == '0');
    while (i >= 0) putchar(a[i--]);
    CLEAR(a);
    puts("");
}

int main() {
    char ch;
    int idx = 0, sp = 0, i, j;
    CLEAR(a), CLEAR(b), CLEAR(rb);
    while (ch = getchar(), ch != EOF) {
        if (ch == ' ' || ch == '\n') {
            if (sp) continue;
            for (i = idx - 1, j = 0; i >= 0; i--, j++) rb[i] = b[j];
            add();
            sp = 1, idx = 0;
            if (ch == '\n') print();
            continue;
        }
        sp = 0;
        b[idx++] = ch;
    }
}
