%{
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
OP     [+\-*\/%]|<=?|!=?|>=?|==?|&&|\|\|
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
<INITIAL>{DELIM}  tokenString("delim", yytext); 
<INITIAL>{OP}     tokenString("op", yytext);
<INITIAL>{KW}     tokenString("kw", yytext);
<INITIAL>{ID}     {
    tokenString("id", yytext);
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
}
<INITIAL>{INT}    tokenString("int", yytext);
<INITIAL>{FLOAT}  tokenString("float", yytext);
<INITIAL>{SCI}    tokenString("sci", yytext);
<INITIAL>{STRING} {
    char token[yyleng];
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

%%

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic)
    {
        printf("frequencies of identifiers:\n");
        for (int i = 0; i < cur_id; i++) {
            printf("%s\t%d\n", id_table[i], id_cnt[i]);
        }
        //format :  printf("%s\t%d\n");
    }

    exit(0);
}
