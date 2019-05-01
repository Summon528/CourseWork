%{
#include "y.tab.h"
#define MAX_LINE_LENGTH 256
#define MAX_ID_LENGTH 256
#define MAX_STRING_LENGTH 256
#define MAX_ID_CNT 4096
#define LIST                { strncat(buf, yytext, MAX_LINE_LENGTH); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }

int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH+1];
char id_table[MAX_ID_CNT][MAX_ID_LENGTH + 1] = {};
int id_cnt[MAX_ID_CNT] = {};
int cur_id = 0;
%}

DELIM  [,;()\[\]{}] 
OP     [+\-*\/%=!]
REL_OP <=?|!=|>=?|==
KW     while|do|if|else|true|false|for|int|print|const|read|boolean|bool|void|float|double|string|continue|break|return
ID     [a-zA-Z][a-zA-Z0-9]*
INT    0|[1-9][0-9]*
FLOAT  [0-9]+\.[0-9]+
SCI    [0-9]+(\.[0-9]+)?[Ee][+\-]?[0-9]+
STRING \"([^"\\\n]|\\.)*\"
SPACE  (" "|\t)+
C_COM  \/\/.*
%Start PRAGMA_COMMENT
%Start COMMENT
%Start PRAGMA_END
%%

<INITIAL>"/*"     LIST; BEGIN COMMENT;
<INITIAL>{DELIM}  {tokenString("delim", yytext); return *yytext;}
<INITIAL>{OP}     {tokenString("op", yytext); return *yytext;}
<INITIAL>{REL_OP} {tokenString("op", yytext); return REL_OP;}
<INITIAL>"&&"     {tokenString("op", yytext); return AND;}
<INITIAL>"||"     {tokenString("op", yytext); return OR;}
<INITIAL>while    {tokenString("kw", yytext); return WHILE;}
<INITIAL>do       {tokenString("kw", yytext); return DO;}
<INITIAL>if       {tokenString("kw", yytext); return IF;}
<INITIAL>else     {tokenString("kw", yytext); return ELSE;}
<INITIAL>true     {tokenString("kw", yytext); return TRUE;}
<INITIAL>false    {tokenString("kw", yytext); return FALSE;}
<INITIAL>for      {tokenString("kw", yytext); return FOR;}
<INITIAL>int      {tokenString("kw", yytext); return INT;}
<INITIAL>print    {tokenString("kw", yytext); return PRINT;}
<INITIAL>const    {tokenString("kw", yytext); return CONST;}
<INITIAL>read     {tokenString("kw", yytext); return READ;}
<INITIAL>boolean  {tokenString("kw", yytext); return BOOLEAN;}
<INITIAL>bool     {tokenString("kw", yytext); return BOOL;}
<INITIAL>void     {tokenString("kw", yytext); return VOID;}
<INITIAL>float    {tokenString("kw", yytext); return FLOAT;}
<INITIAL>double   {tokenString("kw", yytext); return DOUBLE;}
<INITIAL>string   {tokenString("kw", yytext); return STRING;}
<INITIAL>continue {tokenString("kw", yytext); return CONTINUE;}
<INITIAL>break    {tokenString("kw", yytext); return BREAK;}
<INITIAL>return   {tokenString("kw", yytext); return RETURN;}
<INITIAL>{ID}     {
    if (Opt_Statistic) {
        int i;
        for (i = 0; i < cur_id; i++) {
            if (strcmp(yytext, id_table[i]) == 0) {
                id_cnt[i]++;
                break;
            }
        }
        if (i == cur_id) {
            strncpy(id_table[cur_id], yytext, MAX_ID_LENGTH + 1);
            id_cnt[cur_id++] = 1;
        }
    }
    tokenString("id", yytext);
    yylval.text = strdup(yytext);
    return ID;
}
<INITIAL>{INT}    {
    sscanf(yytext, "%d", &yylval.ival);
    tokenString("int", yytext);
    return INT_NUM;
}
<INITIAL>{FLOAT}  {
    sscanf(yytext, "%lf", &yylval.dval);
    tokenString("float", yytext);
    return FLOAT_NUM;
}
<INITIAL>{SCI}    {
    sscanf(yytext, "%lf", &yylval.dval);
    tokenString("sci", yytext);
    return SCI_NUM;
}
<INITIAL>{STRING} {
    char* token = malloc(sizeof(char) * yyleng);
    int t = 0, y = 1;
    while (y < yyleng - 1) {
        if (yytext[y] == '\\' && (yytext[y + 1] == '\\' || yytext[y + 1] == '"')) {
            token[t++] = yytext[++y];
            y++;
            continue;
        }
        token[t++] = yytext[y++];
    }
    token[t] = '\0';
    tokenString("string", token);
    yylval.text = token;
    return STRING_LIT;
}
<INITIAL>{C_COM}  LIST;
<INITIAL>^#pragma{SPACE}source{SPACE}(on|off) {
    LIST;
    Opt_Source = yytext[yyleng - 1] == 'n';
    BEGIN PRAGMA_END;
}
<INITIAL>^#pragma{SPACE}statistic{SPACE}(on|off) {
    LIST;
    Opt_Statistic = yytext[yyleng - 1] == 'n';
    BEGIN PRAGMA_END;
}
<INITIAL>^#pragma{SPACE}token{SPACE}(on|off) {
    LIST;
    Opt_Token = yytext[yyleng - 1] == 'n';
    BEGIN PRAGMA_END;
}

<COMMENT>"*/"        LIST; BEGIN INITIAL;
<COMMENT>.           LIST;
 
<PRAGMA_END>"/*"     LIST; BEGIN PRAGMA_COMMENT;
<PRAGMA_END>\n       BEGIN INITIAL; REJECT;
<PRAGMA_END>{C_COM}  LIST;

<PRAGMA_COMMENT>"*/" LIST; BEGIN PRAGMA_END;
<PRAGMA_COMMENT>\n   BEGIN COMMENT; REJECT;
<PRAGMA_COMMENT>.    LIST;

{SPACE}     LIST;

\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}
