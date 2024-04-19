syntax=syntax.tab.c syntax.tab.h
path=../text/optest5.cmm
all:
	gdb ./parser ${path}

parser: parser.c parser.h lex.yy.c ${syntax} main.c
	gcc syntax.tab.c parser.c main.c -lfl -g -o parser

lex.yy.c:lexical.l
	flex lexical.l

${syntax}:syntax.y
	bison -d -t syntax.y

clean:
	rm lex.yy.c syntax.tab.* parser
