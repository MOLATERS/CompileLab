%{
# include <stdio.h>
# include <stdlib.h>
# include"tree.h"
# include "syntax.tab.h"

extern int lexError;
int lines = 0;
int yycolumn = 1;

#define YY_USER_ACTION \
    yylloc.first_line=yylloc.last_line=yylineno; \
    yylloc.first_column=yycolumn; \
    yylloc.last_column=yycolumn+yyleng-1; \
    yycolumn+=yyleng;
%}
%option yylineno

INT8 [+-]?0[1-7][0-7]*
INT10 [+-]?(0|[1-9][0-9]*)
INT16 [+-]?0[Xx][0-9a-fA-F]+
INT {INT8}|{INT10}|{INT16}
 /*浮点型*/
FLOAT ({INT10}\.[0-9]+)|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+)
 /*数字*/
NUMBER {INT}|{FLOAT}
 /*标识符*/
ID [a-zA-Z_][a-zA-Z0-9_]*
 /*标点*/
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
 /*关键字*/
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
TYPE int|float
 /*注释*/
LINE_COMMENT ("//"[.]*)
BLOCK_COMMENT "/\*"[.]*"\*/"
COMMENT {LINE_COMMENT}|{BLOCK_COMMENT}
 /*空格*/
SPACE [ \n\r\t]+
 /*错误*/
AERROR .
INT8_ERROR 0[0-7]*[8-9]+[0-7]*
INT16_ERROR 0[Xx][a-fA-F0-9]*[g-zG-Z]+[a-fA-F0-9]*
FLOAT_ERROR [0]+(0|[1-9][0-9]*)\.[0-9]+|\.[0-9]+|[0-9]+\.|[0-9]+[Ee][+-]?[0-9]*|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+\.[0-9]+)
NUM_ERROR {INT8_ERROR}|{INT16_ERROR}|{FLOAT_ERROR}
ID_ERROR [0-9]+[a-zA-Z_]+

%%
{SPACE} {}
 /* {COMMENT} {yylval.type_node = addLeaf("COMMENT", 0, yylineno, NULL); return COMMENT;} */
"//" { char c = input(); while (c != '\n') c = input(); }
"/*" {
    int q = 0;
    char c;
    while(1){
        if(q == 0){
            c = input();
            if(c == EOF)
                break;
            if(c == '*')
                q = 1;
        }else{
            c = input();
            if(c == EOF)
                break;
            if(c == '/')
                break;
            else if(c != '*')
                q = 0;
        }
    }
}


{ELSE} { yylval.node = addLeaf(yylineno, "ELSE", TOKEN_OTHER, yytext); return ELSE; }
{WHILE} { yylval.node = addLeaf(yylineno, "WHILE", TOKEN_OTHER, yytext); return WHILE; }
{TYPE} { yylval.node = addLeaf(yylineno, "TYPE", TOKEN_TYPE, yytext); return TYPE; }
{STRUCT} { yylval.node = addLeaf(yylineno, "STRUCT", TOKEN_OTHER, yytext); return STRUCT; }
{RETURN} { yylval.node = addLeaf(yylineno, "RETURN", TOKEN_OTHER, yytext); return RETURN; }
{RELOP} { yylval.node = addLeaf(yylineno, "RELOP", TOKEN_OTHER, yytext); return RELOP; }
{IF} { yylval.node = addLeaf(yylineno, "IF", TOKEN_OTHER, yytext); return IF; }
{PLUS} { yylval.node = addLeaf(yylineno, "PLUS", TOKEN_OTHER, yytext); return PLUS; }
{MINUS} { yylval.node = addLeaf(yylineno, "MINUS", TOKEN_OTHER, yytext); return MINUS; }
{STAR} { yylval.node = addLeaf(yylineno, "STAR", TOKEN_OTHER, yytext); return STAR; }
{DIV} { yylval.node = addLeaf(yylineno, "DIV", TOKEN_OTHER, yytext); return DIV; }
{AND} { yylval.node = addLeaf(yylineno, "AND", TOKEN_OTHER, yytext); return AND; }
{OR} { yylval.node = addLeaf(yylineno, "OR", TOKEN_OTHER, yytext); return OR; }
{NOT} { yylval.node = addLeaf(yylineno, "NOT", TOKEN_OTHER, yytext); return NOT; }

{DOT} { yylval.node = addLeaf(yylineno, "DOT", TOKEN_OTHER, yytext); return DOT; }
{SEMI} { yylval.node = addLeaf(yylineno, "SEMI", TOKEN_OTHER, yytext); return SEMI; }
{COMMA} { yylval.node = addLeaf(yylineno, "COMMA", TOKEN_OTHER, yytext); return COMMA; }
{ASSIGNOP} { yylval.node = addLeaf(yylineno, "ASSIGNOP", TOKEN_OTHER, yytext); return ASSIGNOP; }

{LP} { yylval.node = addLeaf(yylineno, "LP", TOKEN_OTHER, yytext); return LP; }
{RP} { yylval.node = addLeaf(yylineno, "RP", TOKEN_OTHER, yytext); return RP; }
{LB} { yylval.node = addLeaf(yylineno, "LB", TOKEN_OTHER, yytext); return LB; }
{RB} { yylval.node = addLeaf(yylineno, "RB", TOKEN_OTHER, yytext); return RB; }
{LC} { yylval.node = addLeaf(yylineno, "LC", TOKEN_OTHER, yytext); return LC; }
{RC} { yylval.node = addLeaf(yylineno, "RC", TOKEN_OTHER, yytext); return RC; }

{ID} { yylval.node = addLeaf(yylineno, "ID", TOKEN_ID, yytext); return ID;}
{INT} { yylval.node = addLeaf(yylineno, "INT", TOKEN_INT, yytext); return INT;}
{FLOAT} { yylval.node = addLeaf(yylineno, "FLOAT", TOKEN_FLOAT, yytext); return FLOAT;}


{AERROR} { 
    lexError = 1;
    printf("error type A at line %d: mysterious character '%s'\n", yylineno, yytext);
}
%%