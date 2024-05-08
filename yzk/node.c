#include "node.h"
pNode newNode(char* val, char* name, NodeType type, int lineNo, int num, pNode childList[]) {
    pNode curNode = NULL;
    curNode = (pNode)malloc(sizeof(treeNode));
    if (!curNode) {
        printf("节点内存分配失败");
        return NULL;
    }
    if (val!=NULL){
        int valLength = strlen(val) + 1;
        curNode->val = (char*)malloc(sizeof(char) * valLength);
        if (!curNode->val) {
                printf("val内存分配失败");
                free(curNode->val); 
                free(curNode); 
                return NULL;
        }
        strncpy(curNode->val, val, valLength);
    }
    //assert(curNode != NULL);
    int nameLength = strlen(name) + 1;
    curNode->name = (char*)malloc(sizeof(char) * nameLength);
    if (!curNode->name) {
                printf("name内存分配失败");
                free(curNode->name); 
                free(curNode); 
                return NULL;
        }
    strncpy(curNode->name, name, nameLength);
    curNode->lineNo = lineNo;
    curNode->type = type;
    curNode->child = NULL;
    curNode->next = NULL;
    if (childList&&childList[0]) {
        curNode->child = childList[0];
        pNode temp = childList[0];
        for (int i = 1; i < num; i++) {
            if (childList[i]) {
                
                temp->next = childList[i];
                temp = temp->next;
            }
        }
    }
    // if (!strcmp(name,"VarDec") && !strcmp(curNode->child->name,"ID"))
    // {
    //     int valLength = strlen(curNode->child->val) + 1;
    //     curNode->val = (char*)malloc(sizeof(char) * valLength);
    //     if (!curNode->val) {
    //             printf("val内存分配失败");
    //             free(curNode->val); 
    //             free(curNode); 
    //             return NULL;
    //     }
    //     strncpy(curNode->val, curNode->child->val, valLength);
    // }

    return curNode;
}
void delNode(pNode node) {
    if (node == NULL) return;
    while (node->child != NULL) {
        pNode temp = node->child;
        node->child = node->child->next;
        delNode(temp);
    }
    if (node->name!=NULL)
    free(node->name);
    if (node->val!=NULL)
    free(node->val);
    free(node);
    node->name = NULL;
    node->val = NULL;
    node = NULL;
}
void printTreeInfo(pNode curNode, int height) {
    if (curNode == NULL) {
        return;
    }
    for (int i = 0; i < height; i++) {
        printf("  ");
    }
    printf("%s", curNode->name);
    if (curNode->type == NOT_A_TOKEN) {
        printf(" (%d)", curNode->lineNo);
    } else if (curNode->type == TOKEN_ID ||
               curNode->type == TOKEN_INT) {
        printf(": %s", curNode->val);
    } else if (curNode->type == TOKEN_FLOAT) {
        printf(": %lf", atof(curNode->val));
    }
    printf("\n");
    printTreeInfo(curNode->child, height + 1);
    printTreeInfo(curNode->next, height);
}
