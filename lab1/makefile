parser: syntax.tab.c tree.c syntax.tab.h tree.h lex.yy.c
	gcc syntax.tab.c tree.c -lfl -o parser

lex.yy.c:lexical.l
	flex lexical.l

syntax.tab.c syntax.tab.h:syntax.y
	bison -d syntax.y

clean:
	rm lex.yy.c syntax.tab.*
