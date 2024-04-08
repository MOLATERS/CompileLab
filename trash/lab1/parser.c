#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
int synError = 0;
int lexError = 0;
extern TNode* root;
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

struct TNode* InsertNode(int lineno, char* name, enum NODE_TYPE type, int argc, ...){
    struct TNode* newNode = (struct TNode*)malloc(sizeof(struct TNode));
    if(newNode == NULL) return NULL;
    newNode->lineno = lineno;
    newNode->name = strdup(name);
    newNode->type = type;
    newNode->child = NULL;
    newNode->brother = NULL;
    va_list args;
    va_start(args, argc);
    if (argc > 0){
        struct TNode* child = va_arg(args, struct TNode*);
        newNode->child = child;
        struct TNode* current = child;
        for(int i =1; i< argc; i++){
            struct TNode* brother = va_arg(args, struct TNode*);
            if(current != NULL){
                current->brother = brother;
            }
            current = brother;
        }
        va_end(args);
        return newNode;
    }
}

struct TNode* InsertLeaf(int lineno, char* name, enum NODE_TYPE type, char* value){
    struct TNode* leaf = (struct TNode*)malloc(sizeof(struct TNode));
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

void PreOrder(struct TNode* node, int layer){
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

void ReleaseNode(struct TNode* node){
    if(node == NULL) return;
    ReleaseNode(node->child);
    ReleaseNode(node->brother);
    if(node->value_string != NULL) free(node->value_string);
    free(node->name);
    free(node);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        yyparse();
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    // yydebug = 1;
    yyrestart(f);
    yyparse();
    if (!lexError && !synError) {
        PreOrder(root, 0);
    }
    ReleaseNode(root);
    return 0;
}
