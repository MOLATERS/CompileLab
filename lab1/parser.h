#ifndef PARSER
#define PARSER

#include "string.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "ctype.h"

#define TOKEN_TYPE 0
#define TOKEN_ID 1
#define TOKEN_INT 2
#define TOKEN_FLOAT 3
#define TOKEN_OTHER 4
#define TOKEN_ILLEGAL 5
#define TOKEN_UNKNOWN 6

typedef struct TNode{
    int lineno;
    char* name;
    int type;
    int value_int;
    float value_float;
    char* value_string;

    struct TNode* child;
    struct TNode* brother;
}TNode,*TreeNode;

struct TNode* InsertNode(int lineno, char* name, int type, int argc, ...);
struct TNode* CreateNode(int lineno, char* name, int type);
int string_to_num(char* text);
struct TNode* InsertLeaf(int lineno, char* name, int type, char* value);
#endif
