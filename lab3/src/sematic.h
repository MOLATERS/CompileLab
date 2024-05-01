#ifndef SEMATIC
#define SEMETIC
#include "methods.h"

/**
 * @brief 根据给定的类型创建一个新的类型。
 * 
 * @param kind 新类型的种类。
 * @param ... 根据类型种类的附加参数。
 * @return TypePointer 指向新创建的类型的指针。
 */
TypePointer newType(TypeName kind, ...);

/**
 * @brief 检查两个类型是否相等。
 * 
 * @param type1 指向第一个类型的指针。
 * @param type2 指向第二个类型的指针。
 * @return int 如果类型相等，则返回1，否则返回0。
 */
int is_equal(TypePointer type1, TypePointer type2);

/**
 * @brief 遍历抽象语法树并执行语义分析。
 * 
 * @param node 指向当前节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void TreeScan(TreeNode node, Stk stack);

/**
 * @brief 处理抽象语法树中的 ExtDef 节点。
 * 
 * @param node 指向 ExtDef 节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void ExDef(TreeNode node, Stk stack);

/**
 * @brief 分析抽象语法树中的 Specifier 节点。
 * 
 * @param node 指向 Specifier 节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 * @return TypePointer 指向类型信息的指针。
 */
TypePointer Specifier(TreeNode node, Stk stack);

/**
 * @brief 分析抽象语法树中的 StructSpecifier 节点。
 * 
 * @param node 指向 StructSpecifier 节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 * @return TypePointer 指向类型信息的指针。
 */
TypePointer StructSpecifier(TreeNode node, Stk stack);

/**
 * @brief 分析抽象语法树中的 ExtDecList 节点。
 * 
 * @param node 指向 ExtDecList 节点的指针。
 * @param speci 指向类型说明符的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void ExtDecList(TreeNode node, TypePointer speci, Stk stack);

/**
 * @brief 分析抽象语法树中的 VarDec 节点。
 * 
 * @param node 指向 VarDec 节点的指针。
 * @param speci 指向类型说明符的指针。
 * @return Item 指向表示变量声明的项目的指针。
 */
Item VarDec(TreeNode node, TypePointer speci);

/**
 * @brief 分析抽象语法树中的 FunDec 节点。
 * 
 * @param node 指向 FunDec 节点的指针。
 * @param retype 指向返回类型的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void FunDec(TreeNode node, TypePointer retype, Stk stack);

/**
 * @brief 分析抽象语法树中的 VarList 节点。
 * 
 * @param node 指向 VarList 节点的指针。
 * @param func 指向函数项目的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void VarList(TreeNode node, Item func, Stack *stack);

/**
 * @brief 检查项目是否表示结构定义。
 * 
 * 此函数用于检查给定的项目是否表示结构定义。如果项目为NULL，或者项目的类型不是结构类型，
 * 或者结构类型的详细信息中包含了结构名（即已经进行了定义），则返回0；否则返回1。
 * 
 * @param item 指向项目的指针。
 * @return int 如果项目表示结构定义，则返回1；否则返回0。
 */
int isStructDef(Item item);

/**
 * @brief 分析抽象语法树中的 Dec 节点。
 * 
 * 此函数用于分析抽象语法树中的 Dec 节点，根据其产生的规则进行变量声明或赋值操作。
 * 如果 Dec 节点仅包含 VarDec，则进行变量声明；如果 Dec 节点包含 VarDec 和 ASSIGNOP Exp，则进行变量声明并赋值。
 * 
 * @param node 指向 Dec 节点的指针。
 * @param specifier 指向类型说明符的指针。
 * @param structInfo 指向结构信息的指针，如果当前处于结构体定义内部，则该值为非NULL。
 * @param stack 用于符号表管理的栈的指针。
 */
void Dec(TreeNode node, TypePointer specifier, Item structInfo, Stack *stack);


/**
 * @brief 分析抽象语法树中的 ParamDec 节点。
 * 
 * @param node 指向 ParamDec 节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 * @return Field 指向表示参数声明的字段的指针。
 */
Field ParamDec(TreeNode node, Stack *stack);

/**
 * @brief 分析抽象语法树中的 CompSt 节点。
 * 
 * @param node 指向 CompSt 节点的指针。
 * @param returnType 指向返回类型的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void CompSt(TreeNode node, TypePointer returnType, Stack *stack);

/**
 * @brief 分析

抽象语法树中的 StmtList 节点。
 * 
 * @param node 指向 StmtList 节点的指针。
 * @param returnType 指向返回类型的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void StmtList(TreeNode node, TypePointer returnType, Stack *stack);

/**
 * @brief 分析抽象语法树中的 Stmt 节点。
 * 
 * @param node 指向 Stmt 节点的指针。
 * @param returnType 指向返回类型的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void Stmt(TreeNode node, TypePointer returnType, Stack *stack);

/**
 * @brief 分析抽象语法树中的 Def 节点。
 * 
 * @param node 指向 Def 节点的指针。
 * @param structInfo 指向结构信息的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void Def(TreeNode node, Item structInfo, Stack *stack);

/**
 * @brief 分析抽象语法树中的 DefList 节点。
 * 
 * @param node 指向 DefList 节点的指针。
 * @param structInfo 指向结构信息的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void DefList(TreeNode node, Item structInfo, Stack *stack);

/**
 * @brief 分析抽象语法树中的 DecList 节点。
 * 
 * @param node 指向 DecList 节点的指针。
 * @param specifier 指向类型说明符的指针。
 * @param structInfo 指向结构信息的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void DecList(TreeNode node, TypePointer specifier, Item structInfo, Stack *stack);

/**
 * @brief 分析抽象语法树中的 Exp 节点。
 * 
 * @param node 指向 Exp 节点的指针。
 * @param stack 用于符号表管理的栈的指针。
 * @return TypePointer 指向类型信息的指针。
 */
TypePointer Exp(TreeNode node, Stack *stack);

/**
 * @brief 分析抽象语法树中的 Args 节点。
 * 
 * @param node 指向 Args 节点的指针。
 * @param funcInfo 指向函数信息的指针。
 * @param stack 用于符号表管理的栈的指针。
 */
void Args(TreeNode node, Item funcInfo, Stack *stack);


#endif