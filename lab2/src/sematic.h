#ifndef SEMATIC
#define SEMETIC
#include "methods.h"
void TreeScan(TreeNode node, Stk stack);

TypePointer newType(TypeName kind, ...);
int is_equal(TypePointer type1, TypePointer type2);

void ExDef(TreeNode node, Stk stack);
TypePointer StructSpecifier(TreeNode node, Stk stack);
TypePointer Specifier(TreeNode node, Stk stack);
int isStructDef(Item item);
void ExtDecList(TreeNode node, TypePointer speci, Stk stack);
Item VarDec(TreeNode node, TypePointer speci);
void FunDec(TreeNode node, TypePointer retype, Stk stack);
void VarList(TreeNode node, Item func, Stack *stack);
Field ParamDec(TreeNode node, Stack *stack);
void CompSt(TreeNode node, TypePointer returnType, Stack *stack);
void StmtList(TreeNode node, TypePointer returnType, Stack *stack);
void Stmt(TreeNode node, TypePointer returnType, Stack *stack);
void Def(TreeNode node, Item structInfo, Stack *stack);
void DefList(TreeNode node, Item structInfo, Stack *stack);
void Dec(TreeNode node, TypePointer specifier, Item structInfo, Stack *stack);
void DecList(TreeNode node, TypePointer specifier, Item structInfo,Stack*stack);
TypePointer Exp(TreeNode node, Stack *stack);
void Args(TreeNode node, Item funcInfo, Stack *stack);
#endif