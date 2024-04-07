#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
// extern treeNode root;
extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);

// todo: 测试完成
int string_to_num(char* text){
    int num = 0;
    int type_10 =(text[0] != '0'|| (text[0]=='0'&& text[1]=='\0')) ? 1 : 0;
    int type_16 =(text[1] == 'x' || text[1] == 'X')? 1 : 0;
    // printf("type_10: %d,type_16: %d\n",type_10,type_16);
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
            // printf("%d , ==%c, ",num,*text);
            text += 1;
        }
        return num;
    }
}

struct TNode* InsertNode(int lineno, char* name, int type, int argc, ...){
    struct TNode* newNode = CreateNode(lineno,strdup(name),type);
    if(newNode == NULL) 
        return NULL;
    va_list args;
    va_start(args, argc);
    if (argc > 0){
        struct TNode* child = va_arg(args, struct TNode*);
        newNode->child = child;
        struct TNode* current = child;
        for(int i =0; i< argc;i++){
            struct TNode* brother = va_arg(args, struct TNode*);
            if(current != NULL){
                current->brother = brother;
            }
        }
        va_end(args);
        return newNode;
    }
}

struct TNode* CreateNode(int lineno, char* name, int type){
    struct TNode* newNode = (struct TNode*)malloc(sizeof(struct TNode));
    if(newNode == NULL) return NULL;
    newNode->lineno = lineno;
    newNode->name = name;
    newNode->type = type;
    newNode->child = NULL;
    newNode->brother = NULL;
    return newNode;
}

struct TNode* InsertLeaf(int lineno, char* name, int type, char* value){
    struct TNode* leaf = CreateNode(lineno, strdup(name),type);
    if(leaf == NULL)return NULL;
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
    if(leaf->name != NULL){
        return NULL;
    }
    return leaf;
}

void PreOrder(struct TNode* node, int layer){
    if(node == NULL)return;
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
            break;
        case TOKEN_INT:
            printf(" %d",node->value_int);
            break;
        default:
            break;
    }
    printf("\n");
    PreOrder(node->brother,layer++);
    PreOrder(node->child,layer);
}

void ReleaseNode(struct TNode* node){
    if(node == NULL) return;
    if(node->name != NULL) free(node->name);
    if(node->value_string != NULL) free(node->value_string);
    Release(node->child);
    Release(node->brother);
    Release(node);
    return;
}

