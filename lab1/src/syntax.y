%{
#include "lex.yy.c"
#include "unistd.h"
#include "stdio.h"
#include "parser.h"

extern int synError;
struct TNode* root;
%}

%union{ // 将所有可能的类型都包含进去
    TreeNode node;
}

%define parse.lac full
%define parse.error verbose
%token <node> INT FLOAT ID
%token <node> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <node> AND OR DOT NOT TYPE LP RP LB RB LC RC
%token <node> STRUCT RETURN IF ELSE WHILE

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec CompSt
%type <node> StmtList Stmt DefList Def DecList Dec Exp Args

%right ASSIGNOP
%left OR AND 
%left RELOP PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP RB LB DOT
%nonassoc ELSE
%nonassoc LOWER_THAN_ELSE

%%
Program: ExtDefList {$$=InsertNode(@$.first_line,"Program",TOKEN_ILLEGAL,1,$1);root=$$;}      
    ;
ExtDefList:         ExtDef ExtDefList {$$ = InsertNode(@$.first_line,"ExtDefList",TOKEN_ILLEGAL,2,$1,$2);}
    |                                                          {$$=NULL;}
    ;
ExtDef:             Specifier ExtDecList SEMI               {$$ = InsertNode(@$.first_line,"ExtDef",TOKEN_ILLEGAL,3,$1,$2,$3);}
    |               Specifier SEMI                          {$$ = InsertNode(@$.first_line,"ExtDef",TOKEN_ILLEGAL,2,$1,$2);}
    |               Specifier FunDec CompSt                 {$$ = InsertNode(@$.first_line,"ExtDef",TOKEN_ILLEGAL,3,$1,$2,$3);}
    |               error SEMI                              {synError=1;  }
    ;
ExtDecList:         VarDec                                  { $$ = InsertNode(@$.first_line, "ExtDecList", TOKEN_ILLEGAL, 1, $1); }
    |               VarDec COMMA ExtDecList                 { $$ = InsertNode(@$.first_line, "ExtDecList", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    ; 
Specifier:          TYPE                                    { $$ = InsertNode(@$.first_line, "Specifier", TOKEN_ILLEGAL, 1, $1); }
    |               StructSpecifier                         { $$ = InsertNode(@$.first_line, "Specifier",  TOKEN_ILLEGAL,1, $1); }
    ; 
StructSpecifier:    STRUCT OptTag LC DefList RC             { $$ = InsertNode(@$.first_line,"StructSpecifier", TOKEN_ILLEGAL, 5, $1, $2, $3, $4, $5); }
    |               STRUCT Tag                              { $$ = InsertNode(@$.first_line,"StructSpecifier",  TOKEN_ILLEGAL,2, $1, $2); }
    ; 
OptTag:             ID                                      { $$ = InsertNode(@$.first_line,"OptTag", TOKEN_ILLEGAL, 1, $1); }
    |                                                       { $$ = NULL; }
    ; 
Tag:                ID                                      { $$ = InsertNode(@$.first_line, "Tag",  TOKEN_ILLEGAL,1, $1); }
    ; 

VarDec:             ID                                      { $$ = InsertNode(@$.first_line, "VarDec",  TOKEN_ILLEGAL,1, $1); }
    |               VarDec LB INT RB                        { $$ = InsertNode(@$.first_line,  "VarDec", TOKEN_ILLEGAL,4, $1, $2, $3, $4); }
    |               error RB                                {synError=1; }
    ;
FunDec:             ID LP VarList RP                        { $$ = InsertNode(@$.first_line, "FunDec", TOKEN_ILLEGAL, 4, $1, $2, $3, $4); }
    |               ID LP RP                                { $$ = InsertNode(@$.first_line,  "FunDec", TOKEN_ILLEGAL,3, $1, $2, $3); }
    |               error RP                                {synError=1; }
    ; 
VarList:            ParamDec COMMA VarList                  { $$ = InsertNode(@$.first_line,  "VarList", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               ParamDec                                { $$ = InsertNode(@$.first_line, "VarList", TOKEN_ILLEGAL, 1, $1); }
    ; 
ParamDec:           Specifier VarDec                        { $$ = InsertNode(@$.first_line,  "ParamDec", TOKEN_ILLEGAL, 2, $1, $2); }
    ; 

CompSt:             LC DefList StmtList RC                  { $$ = InsertNode(@$.first_line, "CompSt", TOKEN_ILLEGAL, 4, $1, $2, $3, $4); }
    |               error RC                                {synError=1; }
    ; 
StmtList:           Stmt StmtList                           { $$ = InsertNode(@$.first_line, "StmtList", TOKEN_ILLEGAL, 2, $1, $2); }
    |                                                       { $$ = NULL; }
    ; 
Stmt:               Exp SEMI                                { $$ = InsertNode(@$.first_line,"Stmt", TOKEN_ILLEGAL, 2, $1, $2); }
    |               CompSt                                  { $$ = InsertNode(@$.first_line,"Stmt", TOKEN_ILLEGAL, 1, $1); }
    |               RETURN Exp SEMI                         { $$ = InsertNode(@$.first_line,"Stmt", TOKEN_ILLEGAL, 3, $1, $2, $3); }    
    |               IF LP Exp RP Stmt                       { $$ = InsertNode(@$.first_line, "Stmt", TOKEN_ILLEGAL, 5, $1, $2, $3, $4, $5); }
    |               IF LP Exp RP Stmt ELSE Stmt             { $$ = InsertNode(@$.first_line,"Stmt", TOKEN_ILLEGAL, 7, $1, $2, $3, $4, $5, $6, $7); }
    |               WHILE LP Exp RP Stmt                    { $$ = InsertNode(@$.first_line,"Stmt",  TOKEN_ILLEGAL,5, $1, $2, $3, $4, $5); }
    |               error SEMI                              {synError=1; }
    ; 

DefList:            Def DefList                             { $$ = InsertNode(@$.first_line,  "DefList", TOKEN_ILLEGAL, 2, $1, $2); }
    |                                                       { $$ = NULL; }
    ;     
Def:                Specifier DecList SEMI                  { $$ = InsertNode(@$.first_line,  "Def", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    ; 
DecList:            Dec                                     { $$ = InsertNode(@$.first_line,  "DecList", TOKEN_ILLEGAL, 1, $1); }
    |               Dec COMMA DecList                       { $$ = InsertNode(@$.first_line,  "DecList", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    ; 
Dec:                VarDec                                  { $$ = InsertNode(@$.first_line,  "Dec", TOKEN_ILLEGAL, 1, $1); }
    |               VarDec ASSIGNOP Exp                     { $$ = InsertNode(@$.first_line,  "Dec", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    ; 

Exp:                Exp ASSIGNOP Exp                        { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp AND Exp                             { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp OR Exp                              { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp RELOP Exp                           { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp PLUS Exp                            { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp MINUS Exp                           { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp STAR Exp                            { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp DIV Exp                             { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               LP Exp RP                               { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               MINUS Exp                               { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 2, $1, $2); }
    |               NOT Exp                                 { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 2, $1, $2); }
    |               ID LP Args RP                           { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 4, $1, $2, $3, $4); }
    |               ID LP RP                                { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp LB Exp RB                           { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 4, $1, $2, $3, $4); }
    |               Exp DOT ID                              { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               ID                                      { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 1, $1); }
    |               INT                                     { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 1, $1); }
    |               FLOAT                                   { $$ = InsertNode(@$.first_line,  "Exp", TOKEN_ILLEGAL, 1, $1); }
    ; 
Args :              Exp COMMA Args                          { $$ = InsertNode(@$.first_line,  "Args", TOKEN_ILLEGAL, 3, $1, $2, $3); }
    |               Exp                                     { $$ = InsertNode(@$.first_line,  "Args", TOKEN_ILLEGAL, 1, $1); }
    ; 

%%
void yyerror( const char *msg){
    fprintf(stderr, "Error type B at line %d: %s. at place '%s' \n", yylineno, msg, yytext);
}