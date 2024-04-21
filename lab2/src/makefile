syntax=syntax.tab.c syntax.tab.h
path=../test/test12.cmm

all: parser

parser: parser.c parser.h lex.yy.c ${syntax} main.c methods.h methods.c sematic.h sematic.c
	gcc syntax.tab.c parser.c main.c methods.c sematic.c -lfl -g -o parser

lex.yy.c:lexical.l
	flex lexical.l

${syntax}:syntax.y
	bison -d -t syntax.y

test:parser ${path}
	./parser ${path}

clean:
	rm lex.yy.c syntax.tab.* parser
