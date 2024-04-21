#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
extern int synError;
extern int lexError;
extern TreeNode root;
extern int yylineno;
extern int yydebug;
extern int yyparse();
extern void yyrestart(FILE*);

int string_to_num(char* text){
    int num = 0;
    int type_10 =(text[0] != '0'|| (text[0]=='0'&& text[1]=='\0')) ? 1 : 0;
    int type_16 =(text[1] == 'x' || text[1] == 'X')? 1 : 0;
    if(type_10) return atoi(text);
    else if(type_16){
        text += 2;
        while(*text!= '\0'){
            if(*text >= '0' && *text <= '9'){
                num += num*15 + (*text - '0');
            }
            else{
                num += num*15 + (tolower(*text)-'a' + 10);
            }
            text+=1;
        }
        return num;
    }
    else{
        text += 1;
        while(*text != '\0'){
            num += num*7 + (*text - '0');
            text += 1;
        }
        return num;
    }
}

TreeNode InsertNode(int lineno, char* name, NodeType type, int argc, ...){
    TreeNode newNode = (struct TNode*)malloc(sizeof(struct TNode));
    if(newNode == NULL) return NULL;
    newNode->lineno = lineno;
    newNode->name = strdup(name);
    newNode->type = type;
    newNode->child = NULL;
    newNode->brother = NULL;
    va_list args;
    va_start(args, argc);
    if (argc > 0){
        TreeNode child = va_arg(args, struct TNode*);
        newNode->child = child;
        TreeNode current = child;
        for(int i =1; i < argc; i++){
            current->brother = va_arg(args, struct TNode*);
            if(current->brother != NULL){
                current = current->brother;
            }
        }
        va_end(args);
        return newNode;
    }
}

TreeNode InsertLeaf(int lineno, char* name, NodeType type, char* value){
    TreeNode leaf = (struct TNode*)malloc(sizeof(struct TNode));
    if(leaf == NULL)return NULL;
    leaf->lineno = lineno;
    leaf->name = strdup(name);
    if(leaf->name == NULL) return NULL;
    leaf->type = type;
    leaf->child = NULL;
    leaf->brother = NULL;

    switch(type){
        case TOKEN_FLOAT:
            leaf->value_float = atof(value);
            break;
        case TOKEN_INT:
            leaf->value_int = string_to_num(value);
            break;
        case TOKEN_TYPE:
        case TOKEN_ID:
            leaf->value_string = strdup(value);
            break;
        default:
            break;
    }

    return leaf;
}

void PreOrder(TreeNode node, int layer){
    if(node == NULL) return;
    for(int i =0; i<layer;i++)printf(" ");
    printf("%s",node->name);
    switch(node->type){
        case TOKEN_FLOAT:
            printf(" %f",node->value_float);
            break;
        case TOKEN_TYPE:
        case TOKEN_ID:
            printf(" %s", node->value_string);
            break;
        case TOKEN_ILLEGAL:
            printf(" (%d)", node->lineno);
            // printf(" (%d)", layer);
            break;
        case TOKEN_INT:
            printf(" %d",node->value_int);
            break;
        default:
            break;
    }
    printf("\n");
    PreOrder(node->child,layer+1);
    PreOrder(node->brother,layer);
    return;
}

