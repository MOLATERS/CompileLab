#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "node.h"
#define TABLE_SIZE 100
extern int yylineno;

typedef struct type* pType;
typedef struct fieldList* pFieldList;
typedef struct item* pItem;
typedef struct Stack* pStack;
typedef struct Table* pTable;

//首先对数据类型进行定义
typedef enum {//这是数据类型的种类
    INT_TYPE,
    FLOAT_TYPE,
    STRUCT_TYPE,
    ARRAY,
    FUN
} Kind;

typedef struct type {
    Kind kind;
    union {
        // 基本类型
        Kind basic;

        // 数组类型信息包括元素类型与数组大小构成
        struct {
            pType elem;
            int size;
        } array;

        // 结构体类型信息是一个链表
        struct {
            char* structName;
            pFieldList field;
        } structure;

        struct {
            int argc;          // 参数数量
            pFieldList argv;   // 参数--串起来
            pType returnType;  // 返回值类型
        } function;
    } u;
} Type;

//域代表的是详细信息，还可以接到下一个域，主要用于结构体。
typedef struct fieldList {
    char* name;       // 域的名字
    pType type;       // 域的类型
    pFieldList tail;  // 下一个域
} FieldList;

typedef struct item//用于在空间意义上直观，由field来实际执行功能
{
    //char name;//符号名称
    //pType type;//符号类别，int，float之类的
    pFieldList field;
}item;

//hash表设计
typedef struct entry {
    item* value;
    struct entry* next; // 链接到下一个条目，以处理哈希冲突
} Entry;

typedef struct table {
    Entry* entries[TABLE_SIZE]; // 存储Entry的指针数组
} Table;

//现在开始设计栈

// 定义节点结构体
typedef struct stackNode {  //Node->Table->item
    Table* table;
    struct stackNode* next;
} stackNode;

// 定义栈结构体
typedef struct Stack {
    stackNode* top; // 栈顶指针
} Stack;

char* newString(const char* str); //新建field的函数

pFieldList initializeFieldList(char* name, pType type); // 复制FieldList结构的函数

pFieldList copyFieldList(pFieldList src); // 释放FieldList结构的函数

void deleteFieldList(pFieldList fieldList); //设置当前元素的name

void setFieldListName(pFieldList p, char* newName);

pItem initializeItem(pFieldList field);

void deleteItem(pItem item);

pType copyType(pType src); //创建hash表

Table* create_table(); //初始化hashtable函数

void init_table(Table* table);

unsigned int get_hash_code(char* name);

void insert_entry(Table* table, char* key, pItem value);

item* search_entry(Table* table, char* key);

Stack* create_stack(); // 创建栈

void init_stack(Stack* stack); // 判断栈是否为空

int is_empty(Stack* stack); // 入栈操作

void push(Stack* stack, Table* table);

void pop(Stack* stack); // 获取栈顶元素

Table* peek(Stack* stack); // 清空栈

void clear_stack(Stack* stack);

int conflict(Stack* stack,char*name);

pItem stack_search_item(Stack* stack, char* name);

void deleteType(pType type);

int isStructDef(pItem src);


pType newType(Kind kind, ...);

int checkType(pType type1, pType type2);

void traverseTree(pNode node,Stack* stack);

// 用于语义分析

void ExtDef(pNode node,Stack* stack);

void ExtDecList(pNode node, pType specifier,Stack* stack);

pType Specifier(pNode node,Stack* stack);

pType StructSpecifier(pNode node,Stack* stack);

pItem VarDec(pNode node, pType specifier);

void FunDec(pNode node, pType returnType,Stack* stack);

void VarList(pNode node, pItem func,Stack* stack);

pFieldList ParamDec(pNode node,Stack* stack);

void CompSt(pNode node, pType returnType,Stack* stack);

void StmtList(pNode node, pType returnType,Stack*stack);

void Stmt(pNode node, pType returnType,Stack* stack);

void DefList(pNode node, pItem structInfo,Stack*stack);

void Def(pNode node, pItem structInfo,Stack*stack);

void DecList(pNode node, pType specifier, pItem structInfo,Stack*stack);

void Dec(pNode node, pType specifier, pItem structInfo,Stack* stack);

pType Exp(pNode node,Stack*stack);

void Args(pNode node, pItem funcInfo,Stack*stack);
