#ifndef NODE_H
#define NODE_H

// #include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enum.h"


#define TRUE 1
#define FALSE 0
typedef struct node {
    int lineNo;  
    NodeType type;  
    char* name;    
    char* val;    

    struct node* child;  
    struct node* next;   

} treeNode;

typedef treeNode* pNode;

pNode newNode(char* val,char* name, NodeType type, int lineNo,int num, pNode childList[]) ;
void delNode(pNode node);
void printTreeInfo(pNode curNode, int height);                           


#endif
