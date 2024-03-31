#include "tree.h"
#include "syntax.tab.h"
#include "stdio.h"
extern treeNode root;
extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);

int lexError = 0;
int synError = 0;

int convert2num(char* text){
    int num=0;
    //hex
    if (text[0]=='0'&&(text[1]=='x'||text[1]=='X'))
    {
        text+=2;
        while (*text != '\0')
        {
            if (*text>='0' && *text<='9')
            {
                num = num*16+(*text-'0');
            }
            else{
                num = num*16 + (tolower(*text)-'a'+10);
            }
            text++;
        }
        
    }
    //dec
    else if(text[0]!='0'||(text[0]=='0'&& text[1]=='\0'))
    {
        num = atoi(text);
    }
    else{
        text += 1;
        while (*text != '\0')
        {
            num = num*8+(*text - '0');
            text++;
        }
        
    }
    return num;
}

struct AFT* addNode(int line, char* name, enum TYPE type, int argc, ...) {
    // Allocate new node memory
    struct AFT* newNode = (struct AFT*)malloc(sizeof(struct AFT));
    assert(newNode != NULL); // make sure success allocate

    // init
    newNode->line = line;
    newNode->name = strdup(name); // duplite name
    newNode->type = type;
    newNode->child = NULL;
    newNode->brother = NULL;

    va_list args;
    va_start(args, argc);

    if (argc > 0) {
        struct AFT* firstChild = va_arg(args, struct AFT*);
        newNode->child = firstChild;

        struct AFT* currentChild = firstChild;
        for (int i = 1; i < argc; i++) {
            struct AFT* nextChild = va_arg(args, struct AFT*);
            if (currentChild != NULL) {
                currentChild->brother = nextChild;
            }
            currentChild = nextChild;
        }
    }

    va_end(args);

    return newNode;
}

struct AFT* addLeaf(int line, char* name, enum TYPE type, char* val){
    struct AFT* leafNode = (struct AFT*)malloc(sizeof(struct AFT));
    assert(leafNode != NULL);

    leafNode->line = line;
    leafNode->name = strdup(name);
    leafNode->type = type;
    
    switch (type) {
        case TOKEN_INT:
            leafNode->value.i = convert2num(val); // 将字符串转换为十进制整数
            break;
        case TOKEN_TYPE:
            leafNode->value.id = strdup(val);
            break;
        case TOKEN_FLOAT:
            leafNode->value.f = atof(val); // 将字符串转换为浮点数
            break;
        case TOKEN_ID:
            leafNode->value.id = strdup(val); //id
            break;
        default:
            break;
    }
    assert(leafNode->name != NULL);
    //assert(leafNode->value.id != NULL);
    leafNode->child = NULL; 
    leafNode->brother = NULL;  

    return leafNode;
}
void release(struct AFT* node){
    if (node == NULL) return;

    // 释放所有子节点
    while (node->child != NULL) {
        struct AFT* temp = node->child;
        node->child = node->child->brother; // 移动到下一个兄弟节点
        release(temp); // 递归释放子节点
    }


    // 根据节点类型释放资源
    if (node->type == TOKEN_ID) {
        free(node->value.id); // 释放ID类型的值
    }

    free(node->name); // 释放节点名称
    free(node); // 最后释放节点本身

}

void preorder(struct AFT* node, int layer) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < layer; i++) {
        printf("  ");
    }

    // 打印节点名称
    printf("%s", node->name);

    // 根据节点类型打印值或其他信息
    switch (node->type) {
        case TOKEN_TYPE:
            printf(": %s", node->value.id);
            break;
        case TOKEN_ID:
            printf(": %s", node->value.id); 
            break;
        case TOKEN_INT:
            printf(": %d", node->value.i); 
            break;
        case TOKEN_FLOAT:
            printf(": %f", node->value.f); // 打印float类型的值
            break;
        case NOT_A_TOKEN:
            printf(" (%d)", node->line); // 打印行号信息
            break;
        default:
            break;
    }
    printf("\n");

    preorder(node->child, layer + 1);
    preorder(node->brother, layer);
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

    yyrestart(f);
    yyparse();
    if (!lexError && !synError) {
        preorder(root, 0);
    }
    release(root);
    return 0;
}
