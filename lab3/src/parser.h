#ifndef PARSER
#define PARSER

#include "string.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "ctype.h"

typedef enum NODE_TYPE{
    TOKEN_TYPE,
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_OTHER,
    TOKEN_ILLEGAL
}NodeType;

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

TreeNode InsertLeaf(int lineno, char* name, NodeType type, char* value);
TreeNode InsertNode(int lineno, char* name, NodeType type, int argc, ...);
int string_to_num(char* text);
void ReleaseNode(struct TNode* node);
void PreOrder(struct TNode* node, int layer);
void yyerror( const char *msg);
#endif
