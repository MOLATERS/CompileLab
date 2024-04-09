%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "parser.h"
    #include "syntax.tab.h"

    extern int lexError;
    int lines = 0; // 当前的行号
    int yycolumn = 1; // 当前的列号

    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno;\
        yylloc.first_column = yycolumn;\
        yylloc.last_column = yycolumn + yyleng - 1;\
        yycolumn += yyleng;

%}
%option yylineno
INT8 [+-]?0[1-7][0-7]*
INT10 [+-]?(0|[1-9][0-9]*)
INT16 [+-]?0[Xx][0-9a-fA-F]+
INT {INT8}|{INT10}|{INT16}
FLOAT ({INT10}\.[0-9]+)|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+)
NUMBER {INT}|{FLOAT}
ID [a-zA-Z_][a-zA-Z0-9_]*
SEMI  ;
COMMA ,
ASSIGNOP =
PLUS \+
MINUS \-
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT \!
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
RELOP >|<|>=|<=|==|!=
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
TYPE int|float
LINE_COMMENT ("//"[.]*)
BLOCK_COMMENT "/\*"[.]*"\*/"
COMMENT {LINE_COMMENT}|{BLOCK_COMMENT}
SPACE [\n\r\t ]+
AERROR .
INT8_ERROR 0[0-7]*[8-9]+[0-7]*
INT16_ERROR 0[Xx][a-fA-F0-9]*[g-zG-Z]+[a-fA-F0-9]*
FLOAT_ERROR [0]+(0|[1-9][0-9]*)\.[0-9]+|\.[0-9]+|[0-9]+\.|[0-9]+[Ee][+-]?[0-9]*|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+\.[0-9]+)
NUM_ERROR {INT8_ERROR}|{INT16_ERROR}|{FLOAT_ERROR}
ID_ERROR [0-9]+[a-zA-Z_]+
%%
{SPACE} {}
"//" { char c = input(); while (c != '\n') c = input(); }
"/*" {
    int state = 0;
    char c;
    int flag = 1;
    int front = 0;
    while(flag){
        switch(state){
            case 0:
                c = input();
                if(c == EOF) flag = 0;
                if(c == '*') state = 1; 
                if(c == '/') state = 2;
                break;
            case 1:
                c = input();
                if(c == EOF) flag = 0;
                if(c == '/') {
                    if(front == 0) flag = 0;  
                    else front--;
                }
                else state = 0;
                break;
            case 2:
                c = input();
                if(c == '*') front++;
                state = 0;
                break;
            default:
                break;
        }
    }
}

{ELSE} { yylval.node = InsertLeaf(yylineno, "ELSE", TOKEN_OTHER, yytext); return ELSE; }
{WHILE} { yylval.node = InsertLeaf(yylineno, "WHILE", TOKEN_OTHER, yytext); return WHILE; }
{TYPE} { yylval.node = InsertLeaf(yylineno, "TYPE", TOKEN_TYPE, yytext); return TYPE; }
{STRUCT} { yylval.node = InsertLeaf(yylineno, "STRUCT", TOKEN_OTHER, yytext); return STRUCT; }
{RETURN} { yylval.node = InsertLeaf(yylineno, "RETURN", TOKEN_OTHER, yytext); return RETURN; }
{RELOP} { yylval.node = InsertLeaf(yylineno, "RELOP", TOKEN_OTHER, yytext); return RELOP; }
{IF} { yylval.node = InsertLeaf(yylineno, "IF", TOKEN_OTHER, yytext); return IF; }
{PLUS} { yylval.node = InsertLeaf(yylineno, "PLUS", TOKEN_OTHER, yytext); return PLUS; }
{MINUS} { yylval.node = InsertLeaf(yylineno, "MINUS", TOKEN_OTHER, yytext); return MINUS; }
{STAR} { yylval.node = InsertLeaf(yylineno, "STAR", TOKEN_OTHER, yytext); return STAR; }
{DIV} { yylval.node = InsertLeaf(yylineno, "DIV", TOKEN_OTHER, yytext); return DIV; }
{AND} { yylval.node = InsertLeaf(yylineno, "AND", TOKEN_OTHER, yytext); return AND; }
{OR} { yylval.node = InsertLeaf(yylineno, "OR", TOKEN_OTHER, yytext); return OR; }
{NOT} { yylval.node = InsertLeaf(yylineno, "NOT", TOKEN_OTHER, yytext); return NOT; }

{DOT} { yylval.node = InsertLeaf(yylineno, "DOT", TOKEN_OTHER, yytext); return DOT; }
{SEMI} { yylval.node = InsertLeaf(yylineno, "SEMI", TOKEN_OTHER, yytext); return SEMI; }
{COMMA} { yylval.node = InsertLeaf(yylineno, "COMMA", TOKEN_OTHER, yytext); return COMMA; }
{ASSIGNOP} { yylval.node = InsertLeaf(yylineno, "ASSIGNOP", TOKEN_OTHER, yytext); return ASSIGNOP; }

{LP} { yylval.node = InsertLeaf(yylineno, "LP", TOKEN_OTHER, yytext); return LP; }
{RP} { yylval.node = InsertLeaf(yylineno, "RP", TOKEN_OTHER, yytext); return RP; }
{LB} { yylval.node = InsertLeaf(yylineno, "LB", TOKEN_OTHER, yytext); return LB; }
{RB} { yylval.node = InsertLeaf(yylineno, "RB", TOKEN_OTHER, yytext); return RB; }
{LC} { yylval.node = InsertLeaf(yylineno, "LC", TOKEN_OTHER, yytext); return LC; }
{RC} { yylval.node = InsertLeaf(yylineno, "RC", TOKEN_OTHER, yytext); return RC; }

{ID} { yylval.node = InsertLeaf(yylineno, "ID", TOKEN_ID, yytext); return ID;}
{INT} { yylval.node = InsertLeaf(yylineno, "INT", TOKEN_INT, yytext); return INT;}
{FLOAT} { yylval.node = InsertLeaf(yylineno, "FLOAT", TOKEN_FLOAT, yytext); return FLOAT;}

{AERROR} { 
    lexError = 1;
    // if(strcmp(yytext," ")) printf("is empty ");
    printf("error type A at line %d: mysterious character '%s'\n", yylineno, yytext);
}
%%