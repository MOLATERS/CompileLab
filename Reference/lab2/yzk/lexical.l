%{
#include "syntax.tab.h"
#include "node.h"
#define DINT 0
#define DFLOAT 1
#define DID  2
#define DOTHER 4
extern unsigned lexError;
int lines = 0;
int yycolumn = 1;
int comment = 0;

#define YY_USER_ACTION \
    yylloc.first_line=yylloc.last_line=yylineno; \
    yylloc.first_column=yycolumn; \
    yylloc.last_column=yycolumn+yyleng-1; \
    yycolumn+=yyleng;
%}

%option yylineno

INT10 (0|[1-9][0-9]*)
INT8  0([1-7][0-7]*)?
INTF  0[Xx][0-9a-fA-F]*
INT   {INT10}|{INT8}|{INTF}
FLOAT [-]?([0-9]*\.[0-9]+|[0-9]+\.)([eE][+-]?[0-9]+)?
SPACE [\r\t]+
ENTER \n
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS \-
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT \!
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}

STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
BEGIN "/*"
END "*/"
CONTENT ([^\*]+|([\*]+[^\/\*]+))*
COMMENT {BEGIN}{CONTENT}{END}
ID [a-zA-Z_][a-zA-Z0-9_]*
ERRORINT8 [+-]?0([0-9]*[0|8|9][0-9]*)
ERRORINTF [+-]?0[Xx][0-9a-fA-F]*[g-zG-z]+[0-9a-zA-Z]*
ERRORFLOAT [+-]?([0-9]*\.[0-9]+|[0-9]+\.)([eE][+-]?)
ERRORTOKEN [^a-zA-Z0-9_\+\-\*\/&\|\.\!\(\)\[\]\{\}=,;\r\t\n ]*
%%

{INT} { yylval.node = newNode(yytext, "INT",  DINT, yylineno,0,NULL); return INT; }
{FLOAT} { yylval.node = newNode(yytext, "FLOAT", DFLOAT, yylineno,0,NULL); return FLOAT; }
{SEMI} { yylval.node = newNode(yytext, "SEMI",  DOTHER, yylineno,0,NULL); return SEMI; }
{COMMA} { yylval.node = newNode(yytext, "COMMA", DOTHER, yylineno,0,NULL); return COMMA; }
{ASSIGNOP} { yylval.node = newNode(yytext, "ASSIGNOP", DOTHER, yylineno,0,NULL); return ASSIGNOP; }
{RELOP} { yylval.node = newNode(yytext, "RELOP",  DOTHER, yylineno,0,NULL); return RELOP; }
{PLUS} { yylval.node = newNode(yytext, "PLUS",  DOTHER, yylineno,0,NULL); return PLUS; }
{MINUS} { yylval.node = newNode(yytext, "MINUS",  DOTHER, yylineno,0,NULL); return MINUS; }
{STAR} { yylval.node = newNode(yytext, "STAR",  DOTHER, yylineno,0,NULL); return STAR; }
{DIV} { yylval.node = newNode(yytext, "DIV", DOTHER, yylineno,0,NULL); return DIV; }
{AND} { yylval.node = newNode(yytext, "AND",  DOTHER, yylineno,0,NULL); return AND; }
{OR} { yylval.node = newNode(yytext, "OR",  DOTHER, yylineno,0,NULL); return OR; }
{DOT} { yylval.node = newNode(yytext, "DOT",  DOTHER, yylineno,0,NULL); return DOT; }
{NOT} { yylval.node = newNode(yytext, "NOT",  DOTHER, yylineno,0,NULL); return NOT; }
{TYPE} { yylval.node = newNode(yytext, "TYPE",  DOTHER, yylineno,0,NULL); return TYPE; }
{LP} { yylval.node = newNode(yytext, "LP",  DOTHER, yylineno,0,NULL); return LP; }
{RP} { yylval.node = newNode(yytext, "RP",  DOTHER, yylineno,0,NULL); return RP; }
{LB} { yylval.node = newNode(yytext, "LB", DOTHER, yylineno,0,NULL); return LB; }
{RB} { yylval.node = newNode(yytext, "RB", DOTHER, yylineno,0,NULL); return RB; }
{LC} { yylval.node = newNode(yytext, "LC", DOTHER, yylineno,0,NULL); return LC; }
{RC} { yylval.node = newNode(yytext, "RC", DOTHER,yylineno,0,NULL); return RC; }
{COMMENT} {printf("匹配到注释,第%d行,%s\n",yylineno,yytext);}
{ENTER} {yycolumn = 1;}
{STRUCT} { yylval.node = newNode(yytext, "STRUCT", DOTHER, yylineno,0,NULL); return STRUCT; }
{RETURN} { yylval.node = newNode(yytext, "RETURN",  DOTHER, yylineno,0,NULL); return RETURN; }
{IF} { yylval.node = newNode(yytext, "IF", DOTHER, yylineno,0,NULL); return IF; }
{ELSE} { yylval.node = newNode(yytext, "ELSE", DOTHER, yylineno,0,NULL); return ELSE; }
{WHILE} { yylval.node = newNode(yytext, "WHILE",  DOTHER, yylineno,0,NULL); return WHILE; }
{ID} { yylval.node = newNode(yytext, "ID",TOKEN_ID, yylineno,0,NULL); return ID; }

{ERRORINT8} {lexError = TRUE;printf("Error type A at Line %d: Illegal octal number\"%s\".\n", yylineno, yytext); }
{ERRORINTF} {lexError = TRUE;printf("Error type A at Line %d: Illegal  hexadecimal number\"%s\".\n", yylineno, yytext); }
{ERRORFLOAT} {lexError = TRUE; printf("Error type A at Line %d: Illegal  floating point number\"%s\".\n", yylineno, yytext); }
{ERRORTOKEN} {lexError=TRUE;printf("Error type A at Line %d: Illegal  Mysterious character\"%s\".\n", yylineno, yytext); }
%%
