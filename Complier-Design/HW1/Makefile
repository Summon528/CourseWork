all: lex.l
	flex lex.l
	gcc lex.yy.c -o scanner -lfl
clean:
	rm -f lex.yy.c scanner
