TARGET = parser
CC = gcc -g -Wall
LEX = flex
YACC = yacc
YACCFLAG = -d
LIBS = -lfl
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
OBJ+=lex.yy.o
OBJ+=y.tab.o

all: $(TARGET)

parser: $(OBJ) 
	$(CC) $^ -o $(TARGET) $(LIBS)

y.tab.c y.tab.h: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.c: lex.l
	$(LEX) lex.l

lex.yy.o: y.tab.h lex.yy.c
	$(CC) -o lex.yy.o lex.yy.c -c

%.o: %.c
	$(CC) -o $@ $< -c

clean:
	rm -f $(TARGET) $(OBJ) lex.yy.c y.tab.c y.tab.h
