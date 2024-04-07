%{
    #include <stdio.h>
    #include <stdlib.h>
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
ND &&
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
SPACE [\n\r\t]+
AERROR .
INT8_ERROR 0[0-7]*[8-9]+[0-7]*
INT16_ERROR 0[Xx][a-fA-F0-9]*[g-zG-Z]+[a-fA-F0-9]*
FLOAT_ERROR [0]+(0|[1-9][0-9]*)\.[0-9]+|\.[0-9]+|[0-9]+\.|[0-9]+[Ee][+-]?[0-9]*|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+\.[0-9]+)
NUM_ERROR {INT8_ERROR}|{INT16_ERROR}|{FLOAT_ERROR}
ID_ERROR [0-9]+[a-zA-Z_]+
%%
