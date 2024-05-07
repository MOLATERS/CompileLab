syntax=syntax.tab.h syntax.tab.c
includefile=parser.h methods.h sematic.h inter.h syntax.tab.h
srcfile=parser.c lex.yy.c main.c methods.c sematic.c inter.c syntax.tab.c

all: parser

parser:${includefile} ${srcfile}   
	gcc ${srcfile} -lfl -g -o parser

lex.yy.c:lexical.l
	flex lexical.l

${syntax}:syntax.y
	bison -d -t syntax.y

clean:
	rm lex.yy.c syntax.tab.* parser
