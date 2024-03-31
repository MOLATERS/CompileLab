lex.yy.c: hello.l
	flex hello.l

scanner: lex.yy.c
	gcc lex.yy.c -lfl -o scanner

clean:
	rm scanner lex.yy.c

