#include "sematic.h"

TypePointer newType(TypeName kind, ...)
{
    TypePointer p = (TypePointer)malloc(sizeof(Type));
    assert(p != NULL);
    p->kind = kind;
    va_list v_l;
    va_start(v_l, kind);
    switch (kind)
    {
    case INT_TYPE:
    case FLOAT_TYPE:
        p->detail.basic_type = kind;
        break;
    case ARRAY_TYPE:
        p->detail.array.type = va_arg(v_l, TypePointer);
        p->detail.array.size = va_arg(v_l, int);
        break;
    case STRUCT_TYPE:
        p->detail.stuc.name = va_arg(v_l, char *);
        p->detail.stuc.field = va_arg(v_l, Field);
        break;
    case FUNC_TYPE:
        p->detail.func.argc = va_arg(v_l, int);
        p->detail.func.argv = va_arg(v_l, Field);
        p->detail.func.type = va_arg(v_l, TypePointer);
        break;
    }
    va_end(v_l);
    return p;
}

int is_equal(TypePointer type1, TypePointer type2)
{
    if (type1 == NULL || type2 == NULL)
        return 1;
    if (type1->kind != type2->kind)
        return 0;
    if (type1->kind == FUNC_TYPE)
        return 0;
    switch (type1->kind)
    {
    case INT_TYPE:
    case FLOAT_TYPE:
        return type1->detail.basic_type == type2->detail.basic_type;
    case ARRAY_TYPE:
        return is_equal(type1->detail.array.type, type2->detail.array.type);
    case STRUCT_TYPE:
        return !strcmp(type1->detail.stuc.name, type2->detail.stuc.name);
    }
}

void TreeScan(TreeNode node, Stk stack)
{
    if (node == NULL)
        return;
    if (!strcmp(node->name, "ExtDef"))
        ExDef(node, stack);
    TreeScan(node->child, stack);
    TreeScan(node->brother, stack);
}

// ExtDef -> Specifier ExtDecList SEMI
//         | Specifier SEMI
//         | Specifier FunDec CompSt
void ExDef(TreeNode node, Stk stack)
{
    assert(node != NULL);
    TypePointer stype = Specifier(node->child, stack);
    char *secondName = node->child->brother->name;
    // ExtDef -> Specifier ExtDecList SEMI
    if (!strcmp(secondName, "ExtDecList"))
    {
        ExtDecList(node->child->brother, stype, stack);
    }
    // ExtDef -> Specifier FunDec CompSt
    else if (!strcmp(secondName, "FunDec"))
    {
        FunDec(node->child->brother, stype, stack);
        CompSt(node->child->brother->brother, stype, stack);
        Pop(stack);
        Pop(stack);
    }
}

// Specifier -> TYPE
//            | StructSpecifier
TypePointer Specifier(TreeNode node, Stk stack)
{
    assert(node != NULL);
    TreeNode child = node->child;
    if (!strcmp(child->name, "TYPE"))
    {
        if (!strcmp(child->value_string, "float"))
        {
            return newType(FLOAT_TYPE);
        }
        else
        {
            return newType(INT_TYPE);
        }
    }
    return StructSpecifier(child, stack);
}

// StructSpecifier -> STRUCT OptTag LC DefList RC
//                  | STRUCT Tag
TypePointer StructSpecifier(TreeNode node, Stk stack)
{
    assert(node != NULL);
    TypePointer retype = NULL;
    TreeNode temp = node->child->brother;
    // StructSpecifier -> STRUCT OptTag LC DefList RC
    if (strcmp(temp->name, "Tag"))
    {
        Item struct_item = initItem(initFieldList("", newType(STRUCT_TYPE, NULL, NULL)));
        if (!strcmp(temp->name, "OptTag"))
        {
            nameFieldList(struct_item->field, temp->child->value_string);
            temp = temp->brother;
        }
        else
        {
            char structName[20] = {0};
            nameFieldList(struct_item->field, structName);
        }
        if (!strcmp(temp->brother->name, "DefList"))
        {
            DefList(temp->brother, struct_item, stack);
        }
        else
        {
            retype = newType(STRUCT_TYPE, strdup(struct_item->field->name), copyFieldList(struct_item->field->type->detail.stuc.field));
            if (!strcmp(node->child->brother->name, "OptTag"))
            {
                insertItem(Top(stack), struct_item->field->name, struct_item);
            }
            else
            {
                deleteItem(struct_item);
            }
        }
    }
    // StructSpecifier->STRUCT Tag
    else
    {
        Item struct_item = stack_search_item(stack, temp->child->value_string);
        retype = newType(STRUCT_TYPE, strdup(struct_item->field->name), copyFieldList(struct_item->field->type->detail.stuc.field));
    }
    return retype;
}

// ready to delete
int isStructDef(Item item)
{
    if (
        item == NULL ||
        item->field->type->kind != STRUCT_TYPE ||
        item->field->type->detail.stuc.name)
        return 0;
    return 1;
}

// ExtDecList -> VarDec
//             | VarDec COMMA ExtDecList
void ExtDecList(TreeNode node, TypePointer speci, Stk stack)
{
    assert(node != NULL);
    // ExtDecList -> VarDec
    TreeNode temp = node;
    while (temp)
    {
        Item item = VarDec(temp->child, speci);
        if (conflict(stack, item->field->name))
        {
            printf("Error type 3  at Line %d: Redefined variable %s.\n", temp->lineno, item->field->name);
            deleteItem(item);
        }
        else
        {
            insertItem(stack->top->table, item->field->name, item);
        }
        if (temp->child->brother != NULL)
        {
            temp = temp->brother->brother->child;
        }
        else
        {
            break;
        }
    }
}

// VarDec -> ID
//         | VarDec LB INT RB
Item VarDec(TreeNode node, TypePointer speci)
{
    assert(node != NULL);
    TreeNode temp = node;
    while (temp->child)
        temp = temp->child;
    Item item = initItem(initFieldList(temp->value_string, NULL));
    if (!strcmp(node->child->name, "ID"))
    {
        item->field->type = copyType(speci);
    }
    // VarDec -> VarDec LB INT RB
    else
    {
        TreeNode vardec = node->child;
        TypePointer t = speci;
        while (vardec->brother)
        {
            item->field->type = newType(ARRAY_TYPE, copyType(t), vardec->brother->brother->value_int);
            t = item->field->type;
            vardec = vardec->child;
        }
    }
    return item;
}

// FunDec -> ID LP VarList RP
//         | ID LP RP
void FunDec(TreeNode node, TypePointer retype, Stk stack)
{
    assert(node != NULL);
    Item temp = initItem(initFieldList(node->child->value_string, newType(FUNC_TYPE, 0, NULL, copyType(retype))));
    if (conflict(stack, temp->field->name))
    {
        deleteItem(temp);
        temp = NULL;
    }
    else
    {
        insertItem(Top(stack), temp->field->name, temp);
    }
    if (!strcmp(node->child->brother->brother->name, "VarList"))
    {
        VarList(node->child->brother->brother, temp, stack);
        Push(stack, initTable());
    }
}

// VarList -> ParamDec COMMA VarList
//          | ParamDec
void VarList(TreeNode node, Item func, Stack *stack)
{
    assert(node != NULL);
    Push(stack, initTable());
    int argc = 0;
    TreeNode temp = node->child;
    Field cur = NULL;
    // VarList -> ParamDec
    Field paramDec = ParamDec(temp, stack);
    func->field->type->detail.func.argv = copyFieldList(paramDec);
    cur = func->field->type->detail.func.argv;
    argc++;
    // VarList -> ParamDec COMMA VarList
    while (temp->brother)
    {
        temp = temp->brother->brother->child;
        paramDec = ParamDec(temp, stack);
        if (paramDec)
        {
            cur->next = copyFieldList(paramDec); // 把参数穿起来
            cur = cur->next;
            argc++;
        }
    }
    func->field->type->detail.func.argc = argc;
}

// ParamDec -> Specifier VarDec
Field ParamDec(TreeNode node, Stack *stack)
{
    assert(node != NULL);
    TypePointer specifierType = Specifier(node->child, stack);
    Item p = VarDec(node->child->brother, specifierType);
    // ParamDec -> Specifier VarDec
    if (specifierType)
        deleteType(specifierType);
    if (conflict(stack, p->field->name))
    {
        printf("Error type 3  at Line %d: Redefined variable %s.\n", node->lineno, p->field->name);
        deleteItem(p);
        return NULL;
    }
    else
    {
        insertItem(Top(stack), p->field->name, p);
        return p->field;
    }
}

// CompSt -> LC DefList StmtList RC
void CompSt(TreeNode node, TypePointer returnType, Stack *stack)
{
    assert(node != NULL);
    Push(stack, initTable());
    TreeNode temp = node->child->brother;
    if (!strcmp(temp->name, "DefList"))
    {
        DefList(temp, NULL, stack);
        temp = temp->brother;
    }
    if (!strcmp(temp->name, "StmtList"))
    {
        StmtList(temp, returnType, stack);
    }
}

// StmtList -> Stmt StmtList
//           | e
void StmtList(TreeNode node, TypePointer returnType, Stack *stack)
{
    while (node)
    {
        Stmt(node->child, returnType, stack);
        node = node->child->brother;
    }
    return;
}

// Stmt -> Exp SEMI
//       | CompSt
//       | RETURN Exp SEMI
//       | IF LP Exp RP Stmt
//       | IF LP Exp RP Stmt ELSE Stmt
//       | WHILE LP Exp RP Stmt
void Stmt(TreeNode node, TypePointer returnType, Stack *stack)
{
    assert(node != NULL);
    TypePointer expType = NULL;
    // Stmt -> Exp SEMI
    if (!strcmp(node->child->name, "Exp"))
        expType = Exp(node->child, stack);
    // Stmt -> CompSt
    else if (!strcmp(node->child->name, "CompSt"))
    {
        CompSt(node->child, returnType, stack);
        Pop(stack);
    }
    // Stmt -> RETURN Exp SEMI
    else if (!strcmp(node->child->name, "RETURN"))
    {
        expType = Exp(node->child->brother, stack);
    }
    // Stmt -> IF LP Exp RP Stmt
    else if (!strcmp(node->child->name, "IF"))
    {
        TreeNode stmt = node->child->brother->brother->brother->brother;
        expType = Exp(node->child->brother->brother, stack);
        Stmt(stmt, returnType, stack);
        // Stmt -> IF LP Exp RP Stmt ELSE Stmt
        if (stmt->brother != NULL)
            Stmt(stmt->brother->brother, returnType, stack);
    }
    // Stmt -> WHILE LP Exp RP Stmt
    else if (!strcmp(node->child->name, "WHILE"))
    {
        expType = Exp(node->child->brother->brother, stack);
        Stmt(node->child->brother->brother->brother->brother, returnType, stack);
    }
    if (expType)
        deleteType(expType);
}

// Def -> Specifier DecList SEMI
void Def(TreeNode node, Item structInfo, Stack *stack)
{
    assert(node != NULL);
    TypePointer dectype = Specifier(node->child, stack);
    // 你总会得到一个正确的type
    DecList(node->child->brother, dectype, structInfo, stack);
    if (dectype)
        deleteType(dectype);
}

// DefList -> Def DefList
//          | e
void DefList(TreeNode node, Item structInfo, Stack *stack)
{
    while (node)
    {
        Def(node->child, structInfo, stack);
        node = node->child->brother;
    }
}

// Dec -> VarDec
//      | VarDec ASSIGNOP Exp
void Dec(TreeNode node, TypePointer specifier, Item structInfo, Stack *stack)
{
    assert(node != NULL);
    // Dec -> VarDec
    if (node->child->brother == NULL)
    {
        if (structInfo != NULL)
        {
            // 结构体内，将VarDec返回的Item中的filedList
            // Copy判断是否重定义，无错则到结构体链表尾 记得delete掉Item
            Item decitem = VarDec(node->child, specifier);
            Field payload = decitem->field;
            Field structField = structInfo->field->type->detail.stuc.field;
            Field last = NULL;
            while (structField != NULL)
            {
                last = structField;
                structField = structField->next;
            }
            // 新建一个fieldlist,删除之前的item
            if (last == NULL)
            {
                structInfo->field->type->detail.stuc.field =
                    copyFieldList(decitem->field);
            }
            else
            {
                last->next = copyFieldList(decitem->field);
            }
            deleteItem(decitem);
        }
        else
        {
            // 判断返回的item有无冲突，无冲突放入表中，有冲突报错delete
            // printf("VarDec前夕\n");
            Item decitem = VarDec(node->child, specifier); // 我认为这里包含了结构体变量的定义
            if (conflict(stack, decitem->field->name))
            {
                // 出现冲突，报错
                printf("Error type 3  at Line %d: Redefined variable %s.\n", node->lineno,
                       decitem->field->name);
                deleteItem(decitem);
            }
            else
            {
                insertItem(Top(stack), decitem->field->name, decitem);
            }
        }
    }
    // Dec -> VarDec ASSIGNOP Exp
    else
    {
        Item decitem = VarDec(node->child, specifier);
        TypePointer exptype = Exp(node->child->brother->brother, stack);
        if (conflict(stack, decitem->field->name))
        {
            // 出现冲突，报错
            printf("Error type 3  at line %d Redefined variable %s.\n", node->lineno, decitem->field->name);
            deleteItem(decitem);
            return;
        }
        if (!is_equal(decitem->field->type, exptype))
        {
            // 类型不相符
            printf("Error type 5  at line %d Type mismatched for assignment\n",node->lineno);
            deleteItem(decitem);
            return;
        }
        // printf("声明之后1%s\n",decitem->field->name);
        if (decitem->field->type && decitem->field->type->kind == ARRAY_TYPE)
        {
            // 报错，对非basic类型赋值
            deleteItem(decitem);
        }
        else
        {
            insertItem(Top(stack), decitem->field->name, decitem);
        }
        // exp不出意外应该返回一个无用的type，删除
        if (exptype)
            deleteType(exptype);
    }
}

// DecList -> Dec
//          | Dec COMMA DecList
void DecList(TreeNode node, TypePointer specifier, Item structInfo, Stack *stack)
{
    assert(node != NULL);
    TreeNode temp = node;
    while (temp)
    {
        Dec(temp->child, specifier, structInfo, stack);
        if (temp->child->brother)
            temp = temp->child->brother->brother;
        else
            break;
    }
}

// Exp -> Exp ASSIGNOP Exp
//      | Exp AND Exp
//      | Exp OR Exp
//      | Exp RELOP Exp
//      | Exp PLUS Exp
//      | Exp MINUS Exp
//      | Exp STAR Exp
//      | Exp DIV Exp
//      | MINUS Exp
//      | NOT Exp
//      | LP Exp RP
//      | ID LP Args RP
//      | ID LP RP
//      | Exp LB Exp RB
//      | Exp DOT ID
//      | ID
//      | INT
//      | FLOAT
TypePointer Exp(TreeNode node, Stack *stack)
{
    assert(node != NULL);
    TreeNode t = node->child;
    // 二值运算
    if (!strcmp(t->name, "Exp"))
    {
        // 基本数学运算符
        if (strcmp(t->brother->name, "LB") && strcmp(t->brother->name, "DOT"))
        {
            TypePointer p1 = Exp(t, stack);
            TypePointer p2 = Exp(t->brother->brother, stack);
            TypePointer returnType = NULL;
            // Exp -> Exp ASSIGNOP Exp
            if (!strcmp(t->brother->name, "ASSIGNOP"))
            {
                // 检查左值
                TreeNode tchild = t->child;

                if (!strcmp(tchild->name, "FLOAT") ||
                    !strcmp(tchild->name, "INT"))
                {
                    // 报错，左值
                    // printf("左值");
                    printf("Error type 6  at line %d: The left-hand side of an assignment must be a variable.\n", node->lineno);
                }
                else if (!strcmp(tchild->name, "ID") ||
                         !strcmp(tchild->brother->name, "LB") ||
                         !strcmp(tchild->brother->name, "DOT"))
                {
                    if (!is_equal(p1, p2))
                    {
                        // 报错，类型不匹配
                        printf("Error type 5  at line %d: Type mismatched for assignment.\n", node->lineno);
                    }
                    else
                    {
                        returnType = copyType(p1);
                    }
                }
                else
                {
                    // 报错，左值
                    printf("Error type 6  at line %d:  The left-hand side of an assignment must be a variable.\n", node->lineno);
                }
            }
            // Exp -> Exp AND Exp
            //      | Exp OR Exp
            //      | Exp RELOP Exp
            //      | Exp PLUS Exp
            //      | Exp MINUS Exp
            //      | Exp STAR Exp
            //      | Exp DIV Exp
            else
            {
                if (p1 && p2 && (p1->kind == ARRAY_TYPE || p2->kind == ARRAY_TYPE))
                {
                    // 报错，数组，结构体运算
                    //("数组结构体参与二元运算");
                }
                else if (!is_equal(p1, p2))
                {
                    // 报错，类型不匹配
                    printf("Error type 7  at line %d: Type mismatched for operands.",node->lineno);
                }
                else
                {
                    if (p1 && p2)
                    {
                        returnType = copyType(p1);
                    }
                }
            }
            if (p1)
                deleteType(p1);
            if (p2)
                deleteType(p2);
            return returnType;
        }
        // 数组和结构体访问
        else
        {
            // Exp -> Exp LB Exp RB
            if (!strcmp(t->brother->name, "LB"))
            {
                // 数组
                TypePointer p1 = Exp(t, stack);
                TypePointer p2 = Exp(t->brother->brother, stack);
                TypePointer returnType = NULL;
                if (!p1)
                {
                    // 第一个exp为null，上层报错，这里不用再管
                }
                else if (p1 && p1->kind != ARRAY_TYPE)
                {
                    // 报错，非数组使用[]运算符
                    printf("Error type 10 at line %d: \"%s\" is not an array\n", node->lineno, t->child->value_string);
                }
                else if (!p2 || p2->kind != INT_TYPE ||
                         p2->detail.basic_type != INT_TYPE)
                {
                    // 报错，不用int索引[]
                    printf("Error type 12 at line %d: \"%.1f\" is not an integer.\n", node->lineno, t->brother->brother->child->value_float);
                }
                else
                {
                    returnType = copyType(p1->detail.array.type);
                }
                if (p1)
                    deleteType(p1);
                if (p2)
                    deleteType(p2);
                return returnType;
            }
            // Exp -> Exp DOT ID
            else
            {
                TypePointer p1 = Exp(t, stack);
                TypePointer returnType = NULL;
                if (!p1 || p1->kind != STRUCT_TYPE || !p1->detail.stuc.name)
                {
                    // 报错，对非结构体使用.运算符
                }
                else
                { // 查找域名是否存在
                    TreeNode ref_id = t->brother->brother;
                    Field structfield = p1->detail.stuc.field;
                    while (structfield != NULL)
                    {
                        if (!strcmp(structfield->name, ref_id->value_string))
                        {
                            break;
                        }
                        structfield = structfield->next;
                    }
                    if (structfield == NULL)
                    {
                        // 报错，没有可以匹配的域名
                    }
                    else
                    {
                        returnType = copyType(structfield->type);
                    }
                }
                if (p1)
                    deleteType(p1);
                return returnType;
            }
        }
    }
    // 单目运算符
    //  Exp -> MINUS Exp
    //       | NOT Exp
    else if (!strcmp(t->name, "MINUS") || !strcmp(t->name, "NOT"))
    {
        TypePointer p1 = Exp(t->brother, stack);
        TypePointer returnType = NULL;
        if (!p1 || (p1->kind != INT_TYPE && p1->kind != FLOAT_TYPE))
        {
            // 报错，数组，结构体运算
            printf("Error type 7  at Line %d: %s.\n", t->lineno, "Type mismatched for operands");
        }
        else
        {
            returnType = copyType(p1);
        }
        if (p1)
            deleteType(p1);
        return returnType;
    }
    else if (!strcmp(t->name, "LP"))
    {
        return Exp(t->brother, stack);
    }
    // Exp -> ID LP Args RP
    //		| ID LP RP
    else if (!strcmp(t->name, "ID") && t->brother)
    {
        Item funcInfo = stack_search_item(stack, t->value_string);
        assert(funcInfo->field->type->kind == FUNC_TYPE);
        if (funcInfo == NULL)
        {
            char msg[100] = {0};
            printf("Error type 2  at Line %d: Undefined function %s.\n", t->lineno, t->value_string);
            return NULL;
        }
        // Exp -> ID LP Args RP
        else if (!strcmp(t->brother->brother->name, "Args"))
        {
            Args(t->brother->brother, funcInfo, stack);
            return copyType(funcInfo->field->type->detail.func.type);
        }
        // Exp -> ID LP RP
        else
        {
            assert(funcInfo->field->type->detail.func.argc == 0);
            return copyType(funcInfo->field->type->detail.func.type);
        }
    }
    // Exp -> ID
    else if (!strcmp(t->name, "ID"))
    {
        Item tp = stack_search_item(stack, t->value_string);
        if (tp == NULL || isStructDef(tp))
        { // 也不能是函数定义
            printf("Error type 1  at Line %d:  Undefined variable \"%s\".\n", t->lineno, t->value_string);
            return NULL;
        }
        else
        {
            // good
            return copyType(tp->field->type);
        }
    }
    else
    {
        // Exp -> FLOAT
        if (!strcmp(t->name, "FLOAT"))
        {
            return newType(FLOAT_TYPE, FLOAT_TYPE);
        }
        // Exp -> INT
        else
        {
            return newType(INT_TYPE, INT_TYPE);
        }
    }
}

// Args -> Exp COMMA Args
//       | Exp
void Args(TreeNode node, Item funcInfo, Stack *stack)
{
    assert(node != NULL);
    TreeNode temp = node;
    Field arg = funcInfo->field->type->detail.func.argv;
    while (temp)
    {
        assert(arg == NULL);
        TypePointer realType = Exp(temp->child, stack);
        if (realType)
            deleteType(realType);
        arg = arg->next;
        if (temp->child->brother)
        {
            temp = temp->child->brother->brother;
        }
        else
        {
            break;
        }
    }
    assert(arg != NULL);
}
