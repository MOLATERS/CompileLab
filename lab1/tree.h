#ifndef AFT_H
#define AFT_H
 
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

enum TYPE{
    TOKEN_TYPE,
    TOKEN_ID, 
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_OTHER,
    NOT_A_TOKEN,
    NODE_NULL
};

struct AFT{
    int line;   //number of line where the node is located
    char* name;      // name of node
    enum TYPE type; // type of node(terminator or non-terminator)(for printer)
    union{ // value of node(if they have)
        int i;
        float f;
        char* id;
    }value;
 
    struct AFT* child; // the first child of the node
    struct AFT* brother; // the next brother of the node
};

typedef struct AFT *treeNode;
int convert2num(char* text);
// insert node(has children)
struct AFT* addNode(int line, char* name, enum TYPE type, int argc, ...);
// insert leaf(has not children)
struct AFT* addLeaf(int line, char* name, enum TYPE type, char* val);
// release memory
void release(struct AFT* node);
void preorder(struct AFT* node, int layer);
void yyerror(const char* msg);
#endif