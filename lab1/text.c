#include "parser.h"

int string_to_num(char* text){
    int num = 0;
    int type_10 =(text[0] != '0'|| (text[0]=='0'&& text[1]=='\0')) ? 1 : 0;
    int type_16 =(text[1] == 'x' || text[1] == 'X')? 1 : 0;
    printf("type_10: %d,type_16: %d\n",type_10,type_16);
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
            printf("%d , ==%c, ",num,*text);
            text += 1;
        }
        return num;
    }
}

TreeNode InsertNode(int lineno, char* name, int type, int argc, ...){
    struct TNode* newNode = CreateNode(lineno,strdup(name),type);
    if(newNode == NULL) 
        return NULL;
    va_list args;
    va_start(args, argc); // 初始化可变参数列表
    if (argc > 0){
        struct TNode* child = va_arg(args, struct TNode*); // 提取每一个对应的类型变量的值
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

TreeNode CreateNode(int lineno, char* name, int type){
    TreeNode newNode = (struct TNode*)malloc(sizeof(struct TNode));
    newNode->lineno = lineno;
    newNode->name = name;
    newNode->type = type;
    newNode->child = NULL;
    newNode->brother = NULL;
    return newNode;
}

TreeNode InsertLeaf(int linno, char* name, int type, int args, ...){

}

int main(){
    char* text = "032";
    int num = string_to_num(text);
    printf("answer is : %d\n",num);
    char* s = "you";
    InsertNode(0,s,TOKEN_FLOAT, 1, 1,s,TOKEN_ID);
}