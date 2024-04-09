#ifndef PARSER
#define PARSER

#include "string.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "ctype.h"

enum NODE_TYPE{
    TOKEN_TYPE,
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_OTHER,
    TOKEN_ILLEGAL
};

typedef struct TNode{
    int lineno;
    char* name;
    enum NODE_TYPE type;
    int value_int;
    float value_float;
    char* value_string;

    struct TNode* child;
    struct TNode* brother;
}TNode,*TreeNode;

struct TNode* InsertLeaf(int lineno, char* name, enum NODE_TYPE type, char* value);
struct TNode* InsertNode(int lineno, char* name, enum NODE_TYPE type, int argc, ...);
int string_to_num(char* text);
void ReleaseNode(struct TNode* node);
void PreOrder(struct TNode* node, int layer);
void yyerror(const char *msg);
#endif
