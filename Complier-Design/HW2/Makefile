CC=gcc
CFLAGS=-lfl -ly
TARGET=parser
LEX=lex.yy.c
YACC=y.tab.c
YACC_HEADER=y.tab.h

all: $(TARGET)

$(TARGET): $(LEX) $(YACC) $(YACC_HEADER)
	$(CC) $(LEX) $(YACC) -o $(TARGET) $(CFLAGS)

$(LEX): lex.l
	flex lex.l

$(YACC) $(YACC_HEADER): yacc.y
	yacc -d yacc.y

clean:
	-rm $(LEX) $(TARGET) $(YACC) $(YACC_HEADER)
