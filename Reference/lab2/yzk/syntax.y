%{
    #include<stdio.h>
    #include"node.h"
    #include"lex.yy.c"
    extern int synError;
    pNode root;
    #define YYERROR_VERBOSE 1

%}

// types

%union{
    pNode node; 
}
%locations
// tokens

%token <node> INT
%token <node> FLOAT
%token <node> ID
%token <node> TYPE
%token <node> COMMA
%token <node> DOT
%token <node> SEMI
%token <node> RELOP
%token <node> ASSIGNOP
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR NOT 
%token <node> LP RP LB RB LC RC
%token <node> IF
%token <node> ELSE
%token <node> WHILE
%token <node> STRUCT
%token <node> RETURN



%type <node> Program ExtDefList ExtDef ExtDecList   
%type <node> Specifier StructSpecifier OptTag Tag   
%type <node> VarDec FunDec VarList ParamDec         
%type <node> CompSt StmtList Stmt                   
%type <node> DefList Def Dec DecList                
%type <node> Exp Args                              


%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB
%left LP RP

%nonassoc ELSE

%%
Program : ExtDefList 					{pNode childList[1] = {$1}; $$= newNode(NULL,"Program", 4, @$.first_line, 1,childList); root = $$;}
        ;
ExtDefList : ExtDef ExtDefList 			{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"ExtDefList", 4, @$.first_line, 2,childList);}
           | 						{$$= NULL;}
           ;
ExtDef : Specifier ExtDecList SEMI 			{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"ExtDef", 4, @$.first_line,3, childList);}
       | Specifier SEMI 				{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"ExtDef", 4, @$.first_line, 2,childList);}
       | Specifier FunDec CompSt 			{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"ExtDef", 4, @$.first_line,3, childList);}
       ;
ExtDecList : VarDec 					{pNode childList[1] = {$1}; $$= newNode(NULL,"ExtDecList", 4, @$.first_line,1, childList);}
           | VarDec COMMA ExtDecList 			{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"ExtDecList", 4, @$.first_line,3, childList);}
           ;

/*Specifiers*/
Specifier : TYPE 					{pNode childList[1] = {$1}; $$= newNode(NULL,"Specifier", 4, @$.first_line, 1,childList);}
          | StructSpecifier 					{pNode childList[1] = {$1}; $$= newNode(NULL,"Specifier", 4, @$.first_line, 1,childList);}
          ;
StructSpecifier : STRUCT OptTag LC DefList RC 	{pNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode(NULL,"StructSpecifier", 4, @$.first_line,5, childList);}
                | STRUCT Tag 					{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"StructSpecifier", 4, @$.first_line, 2, childList);} 
                ;
OptTag : ID 						{pNode childList[1] = {$1}; $$= newNode(NULL,"OptTag", 4, @$.first_line, 1 ,childList);}
       |  						{$$= NULL;}
       ;
Tag : ID 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Tag", 4, @$.first_line,1 ,childList);}
    ;

/*Declarators*/
VarDec : ID 						{pNode childList[1] = {$1}; $$= newNode(NULL,"VarDec", 4, @$.first_line, 1, childList);}
    | VarDec LB INT RB 				{pNode childList[4] = {$1, $2, $3, $4}; $$= newNode(NULL,"VarDec", 4, @$.first_line, 4, childList);}
    | error RB                      {printf("这里发生了错误");synError = 1;}
    ;
FunDec : ID LP VarList RP 				{pNode childList[4] = {$1, $2, $3, $4}; $$= newNode(NULL,"FunDec", 4, @$.first_line,4 , childList);}
    | ID LP RP  					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"FunDec", 4, @$.first_line,3,  childList);}
    ;
VarList : ParamDec COMMA VarList  			{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"VarList", 4, @$.first_line, 3, childList);}
    | ParamDec 					{pNode childList[1] = {$1}; $$= newNode(NULL,"VarList", 4, @$.first_line, 1, childList);}
    ;
ParamDec : Specifier VarDec 				{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"ParamDec", 4, @$.first_line,2, childList);}
    ;

/* Statements*/
CompSt : LC DefList StmtList RC 			{pNode childList[4] = {$1, $2, $3, $4}; $$= newNode(NULL,"CompSt", 4, @$.first_line, 4,childList);}
       | error RC					{synError = 1;}
       ;
StmtList : Stmt StmtList 				{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"StmtList", 4, @$.first_line, 2,childList);}
         | 						{$$= NULL;}
         ;	
Stmt : Exp SEMI 					{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"Stmt", 4, @$.first_line, 2,childList);}
     | CompSt 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Stmt", 4, @$.first_line,1, childList);}
     | RETURN Exp SEMI  				{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Stmt", 4, @$.first_line,3, childList);}
     | IF LP Exp RP Stmt 				{pNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode(NULL,"Stmt", 4, @$.first_line, 5,childList);}
     | IF LP Exp RP Stmt ELSE Stmt 			{pNode childList[7] = {$1, $2, $3, $4, $5, $6, $7}; $$= newNode(NULL,"Stmt", 4, @$.first_line,7, childList);}
     | WHILE LP Exp RP Stmt 				{pNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode(NULL,"Stmt", 4, @$.first_line,5,childList);}
     | error SEMI 					{synError = 1; }
     ;

/* Local Definitions*/
DefList : Def DefList 					{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"DefList", 4, @$.first_line,2, childList);}
        | 					   	{$$= NULL;}
        ;
Def : Specifier DecList SEMI 				{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Def", 4, @$.first_line,3, childList);}
    ;
DecList : Dec 						{pNode childList[1] = {$1}; $$= newNode(NULL,"DecList", 4, @$.first_line, 1,childList);}
        | Dec COMMA DecList 				{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"DecList", 4, @$.first_line,3, childList);}
        ;
Dec : VarDec 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Dec", 4, @$.first_line,1, childList);}
    | VarDec ASSIGNOP Exp 				{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Dec", 4, @$.first_line,3, childList);}
    ;

/*Expressions*/
Exp : Exp ASSIGNOP Exp 				{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line, 3,childList);}
    | Exp AND Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line, 3,childList);}
    | Exp OR Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line, 3,childList);}
    | Exp RELOP Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3, childList);} 
    | Exp PLUS Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3, childList);}
    | Exp MINUS Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3, childList);}
    | Exp STAR Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3, childList);}
    | Exp DIV Exp 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3,  childList);}
    | LP Exp RP 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3,  childList);}
    | MINUS Exp 						{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"Exp", 4, @$.first_line,2,  childList);}
    | NOT Exp 						{pNode childList[2] = {$1, $2}; $$= newNode(NULL,"Exp", 4, @$.first_line,2, childList);}
    | ID LP Args RP 					{pNode childList[4] = {$1, $2, $3, $4}; $$= newNode(NULL,"Exp", 4, @$.first_line,4,  childList);}
    | ID LP RP 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3,  childList);}
    | Exp LB Exp RB 					{pNode childList[4] = {$1, $2, $3, $4}; $$= newNode(NULL,"Exp", 4, @$.first_line,4,  childList);}
    | Exp DOT ID 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Exp", 4, @$.first_line,3,  childList);}
    | ID 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Exp", 4, @$.first_line,1, childList);}
    | INT 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Exp", 4, @$.first_line,1, childList);}
    | FLOAT 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Exp", 4, @$.first_line,1, childList);}
    //| error RP                      { printf("表达式报错");synError=1;}
    ;

Args : Exp COMMA Args 					{pNode childList[3] = {$1, $2, $3}; $$= newNode(NULL,"Args", 4, @$.first_line,3,  childList);}
    | Exp 						{pNode childList[1] = {$1}; $$= newNode(NULL,"Args", 4, @$.first_line,1, childList);}
    ;

/*ERRORS*/
/*Exp : Exp error LB Exp RB {}
    ;
*/
%%

yyerror(char* msg){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, msg);
}
