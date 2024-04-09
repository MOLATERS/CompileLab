syntax=syntax.tab.c syntax.tab.h
path=text/optest.cmm
all:
	./parser ${path}

parser: parser.c parser.h lex.yy.c ${syntax}
	gcc syntax.tab.c parser.c -lfl -g -o parser

lex.yy.c:lexical.l
	flex lexical.l

${syntax}:syntax.y
	bison -d -t syntax.y

clean:
	rm lex.yy.c syntax.tab.* parser