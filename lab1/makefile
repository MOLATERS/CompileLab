all:
	./parser ./text/optest.cmm

parser: syntax.tab.c parser.c syntax.tab.h parser.h lex.yy.c
	gcc syntax.tab.c parser.c -lfl -g -o parser

lex.yy.c:lexical.l
	flex lexical.l

syntax.tab.c syntax.tab.h:syntax.y
	bison -d -t syntax.y

clean:
	rm lex.yy.c syntax.tab.* parser