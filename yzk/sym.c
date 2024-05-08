#include"sym.h"
//新建field的函数
char* newString(const char* str) {
    return strdup(str);
}
pFieldList initializeFieldList(char* name, pType type) {
    // 分配内存
    pFieldList newList = (pFieldList)malloc(sizeof(FieldList));
    if (newList == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // 分配内存并复制name
    newList->name = strdup(name);
    //printf("分配完毕%s",newList->name);
    if (newList->name == NULL) {
        perror("Memory allocation failed");
        free(newList);
        exit(EXIT_FAILURE);
    }

    // 设置type和tail
    newList->type = type;
    newList->tail = NULL;
    //printf("初始化field函数中接到了%s\n",newList->name);
    return newList;
}

// 复制FieldList结构的函数
pFieldList copyFieldList(pFieldList src) {
    assert(src != NULL);
    pFieldList head = NULL, cur = NULL;
    pFieldList temp = src;

    while (temp) {
        if (!head) {
            head = initializeFieldList(temp->name, copyType(temp->type));
            cur = head;
            temp = temp->tail;
        } else {
            cur->tail = initializeFieldList(temp->name, copyType(temp->type));
            cur = cur->tail;
            temp = temp->tail;
        }
    }
    return head;
}
// 释放FieldList结构的函数
void deleteFieldList(pFieldList fieldList) {
    assert(fieldList != NULL);
    if (fieldList->name) {
        free(fieldList->name);
        fieldList->name = NULL;
    }
    if (fieldList->type) deleteType(fieldList->type);
    fieldList->type = NULL;
    free(fieldList);
}
//设置当前元素的name
void setFieldListName(pFieldList p, char* newName) {
    assert(p != NULL && newName != NULL);
    if (p->name != NULL) {
        free(p->name);
    }
    // int length = strlen(newName) + 1;
    // p->name = (char*)malloc(sizeof(char) * length);
    // strncpy(p->name, newName, length);
    p->name = newString(newName);
}
pType copyType(pType src) {
    if (src == NULL) return NULL;
    pType p = (pType)malloc(sizeof(Type));
    assert(p != NULL);
    p->kind = src->kind;
    assert(p->kind == INT_TYPE ||p->kind == FLOAT_TYPE || p->kind == ARRAY || p->kind == STRUCT_TYPE ||
           p->kind == FUN);
    switch (p->kind) {
        case INT_TYPE:
        case FLOAT_TYPE:
            p->u.basic = src->u.basic;
            break;
        case ARRAY:
            p->u.array.elem = copyType(src->u.array.elem);
            p->u.array.size = src->u.array.size;
            break;
        case STRUCT_TYPE:
            p->u.structure.structName = newString(src->u.structure.structName);
            p->u.structure.field = copyFieldList(src->u.structure.field);
            break;
        case FUN:
            p->u.function.argc = src->u.function.argc;
            p->u.function.argv = copyFieldList(src->u.function.argv);
            p->u.function.returnType = copyType(src->u.function.returnType);
            break;
    }

    return p;
}
pItem initializeItem(pFieldList field) {
    // 分配内存
    //printf("接到了%s\n",field->name);
    pItem newItem = (pItem)malloc(sizeof(item));
    if (newItem == NULL) {
        //printf("内存分配失败");

        exit(-1);
    }

    printf("正在初始化item\n");
    //printf("接到了%s\n",field->name);
    // 设置字段
    newItem->field = field;
    
    return newItem;
}

//创建hash表
Table* create_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) {
        // 内存分配失败
        return NULL;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        table->entries[i] = NULL;
    }
    init_table(table);
    return table;
}

//初始化hashtable函数
void init_table(Table* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->entries[i] = NULL;
    }
}
unsigned int get_hash_code(char* name)
{
    unsigned int val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val%100;
}
void insert_entry(Table* table, char* key, pItem value) {//key就用item field name来搞吧
    unsigned int index = get_hash_code(key);
    //printf("\ninsert_entry index %d key %s\n",index,key);
    // 创建新的Entry
    Entry* new_entry = malloc(sizeof(Entry));
    new_entry->value = value;
    new_entry->next = NULL;

    // 检查该位置是否已经有条目
    if (table->entries[index] == NULL) {
        table->entries[index] = new_entry;
    } else {
        // 在链表尾部添加新的条目
        Entry* current = table->entries[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_entry;
    }
}
item* search_entry(Table* table, char* key) {
    //printf("\nkey:%s\n",key);
    unsigned int index = get_hash_code(key);
    //printf("\nindex%d\n",index);
    Entry* current = table->entries[index];
    // if (current==NULL)
    //     printf("search_entry当前节点不存在\n");
    // else
    //     printf("search_entry当前节点存在\n");
    while (current != NULL) {
        printf("%s",current->value->field->name);
        if (strcmp(current->value->field->name, key) == 0) {
            //printf("\nsearch_entry找到了\n");
            return current->value;

        }
        current = current->next;
    }

    return NULL; // 没有找到匹配的条目
}

// 创建栈
Stack* create_stack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    init_stack(stack); // 初始化栈
    return stack;
}

// 初始化栈
void init_stack(Stack* stack) {
    stack->top = NULL; // 将栈顶指针置为NULL，表示栈为空
}

// 判断栈是否为空
int is_empty(Stack* stack) {
    return stack->top == NULL;
}

// 入栈操作
void push(Stack* stack, Table* table) {
    // 创建新节点
    stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode->table = table;
    newNode->next = stack->top; // 将新节点的next指针指向当前栈顶
    stack->top = newNode;       // 更新栈顶指针为新节点
}

// 出栈操作
void pop(Stack* stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        exit(1);
    }

    Table* data = stack->top->table; // 获取栈顶节点的数据
    stackNode* temp = stack->top;     // 保存栈顶节点的地址以便后面释放内存
    stack->top = stack->top->next; // 更新栈顶指针为下一个节点
    free(temp); // 释放原栈顶节点的内存

    return; // 返回出栈的数据
}

// 获取栈顶元素
Table* peek(Stack* stack) {
    if (is_empty(stack)) {
        printf("栈是空的\n");
        exit(1);
    }
    return stack->top->table; // 返回栈顶节点的数据
}

// 清空栈
void clear_stack(Stack* stack) {
    while (!is_empty(stack)) {
        pop(stack);
    }
}
int conflict(Stack* stack,char*name)
{
    //printf("\n冲突校验\n");
    Table* p = (Table*)malloc(sizeof(Table));
    if (p==NULL) {printf("内存分配失败");return -1;}
    //printf("冲突校验1");
    p = peek(stack);
    //if(p!=NULL) printf("栈顶有元素");
    //printf("冲突校验2");
    if(search_entry(p,name)==NULL)
    {
        //printf("\nconflict表明未发生重定义\n");
        return 0;
    }
    //printf("冲定义了\n");
    return 1;

}
pItem stack_search_item(Stack* stack, char* name) {
    stackNode* current = stack->top;

    while (current != NULL) {
        pItem item = search_entry(current->table, name);
        if (item != NULL) {
            return item; // 找到了匹配的item
        }
        current = current->next; // 继续向下搜索
    }
    printf("栈里没找到\n");
    return NULL; // 没有找到匹配的item
}
void deleteType(pType type) {
    assert(type != NULL);
    pFieldList temp = NULL;
    // pFieldList tDelete = NULL;
    switch (type->kind) {
        case INT_TYPE:
        case FLOAT_TYPE:
            break;
        case ARRAY:
            deleteType(type->u.array.elem);
            type->u.array.elem = NULL;
            break;
        case STRUCT_TYPE:
            if (type->u.structure.structName)
                free(type->u.structure.structName);
            type->u.structure.structName = NULL;

            temp = type->u.structure.field;
            while (temp) {
                pFieldList tDelete = temp;
                temp = temp->tail;
                deleteFieldList(tDelete);
            }
            type->u.structure.field = NULL;
            break;
        case FUN:
            deleteType(type->u.function.returnType);
            type->u.function.returnType = NULL;
            temp = type->u.function.argv;
            while (temp) {
                pFieldList tDelete = temp;
                temp = temp->tail;
                deleteFieldList(tDelete);
            }
            type->u.function.argv = NULL;
            break;
    }
    free(type);
}
void deleteItem(pItem item) {
    assert(item != NULL);
    if (item->field != NULL) deleteFieldList(item->field);
    free(item);
}
int isStructDef(pItem src) {
    if (src == NULL) return 0;
    if (src->field->type->kind != STRUCT_TYPE) return 0;
    if (src->field->type->u.structure.structName) return 0;
    return 1;
}
//数据结构的定义大概就进行到这里
//现在要进行的是程序的编写
pType newType(Kind kind, ...) {
    pType p = (pType)malloc(sizeof(Type));
    assert(p != NULL);
    p->kind = kind;
    va_list vaList;
    assert(kind == INT_TYPE || kind ==  FLOAT_TYPE || kind ==  STRUCT_TYPE ||kind ==  ARRAY ||
           kind ==  FUN);
    switch (kind) {
        case INT_TYPE:
            va_start(vaList, kind);
            p->u.basic = va_arg(vaList, Kind);
            break;
        case FLOAT_TYPE:
            va_start(vaList, kind);
            p->u.basic = va_arg(vaList, Kind);
            break;
        case ARRAY:
            va_start(vaList, kind);
            p->u.array.elem = va_arg(vaList, pType);
            p->u.array.size = va_arg(vaList, int);
            break;
        case STRUCT_TYPE:
            va_start(vaList, kind);
            p->u.structure.structName = va_arg(vaList, char*);
            p->u.structure.field = va_arg(vaList, pFieldList);
            break;
        case FUN:
            va_start(vaList, kind);
            p->u.function.argc = va_arg(vaList, int);
            p->u.function.argv = va_arg(vaList, pFieldList);
            p->u.function.returnType = va_arg(vaList, pType);
            break;
    }
    va_end(vaList);
    return p;
}
int checkType(pType type1, pType type2) {
    if (type1 == NULL || type2 == NULL) return 1;
    if (type1->kind == FUN || type2->kind == FUN) return 0;
    if (type1->kind != type2->kind)
        return 0;
    else {
        assert(type1->kind == INT_TYPE ||type1->kind == FLOAT_TYPE || type1->kind == ARRAY ||
               type1->kind == STRUCT_TYPE);
        switch (type1->kind) {
            case INT_TYPE:
            case FLOAT_TYPE:
                return type1->u.basic == type2->u.basic;
            case ARRAY:
                return checkType(type1->u.array.elem, type2->u.array.elem);
            case STRUCT_TYPE:
                return !strcmp(type1->u.structure.structName,
                               type2->u.structure.structName);
        }
    }
}

void Extlist(pNode node,Stack* stack) {
    if (node == NULL) return;
    if (!strcmp(node->name, "ExtDef")) ExtDef(node,stack);
    Extlist(node->child,stack);
    Extlist(node->next,stack);
}
void ExtDef(pNode node,Stack* stack) {
    assert(node != NULL);
    // ExtDef -> Specifier ExtDecList SEMI
    //         | Specifier SEMI
    //         | Specifier FunDec CompSt
    //printf("hello\n");
    pType specifierType = Specifier(node->child,stack);//在函数中这是函数的返回值，在第一个式子中是变量类型，第二个式子是结构体
    //printf("helloend\n");
    char* secondName = node->child->next->name;

    // ExtDef -> Specifier ExtDecList SEMI
    if (!strcmp(secondName, "ExtDecList")) {
        // TODO: process first situation
        //printf("???");
        ExtDecList(node->child->next, specifierType,stack);
    }
    // ExtDef -> Specifier FunDec CompSt
    else if (!strcmp(secondName, "FunDec")) {
        FunDec(node->child->next, specifierType,stack);
        //printf("执行完函数\n");
        CompSt(node->child->next->next, specifierType,stack);
        //pop(stack);
        //pop(stack);
    }
    if (specifierType) deleteType(specifierType);//不用这个定义头了，这里应该开个列表吧，不要新压栈
}
void ExtDecList(pNode node, pType specifier,Stack* stack) {
    assert(node != NULL);
    // ExtDecList -> VarDec
    //             | VarDec COMMA ExtDecList
    pNode temp = node;
    while (temp) {
        pItem item = VarDec(temp->child, specifier);
        if (conflict(stack, item->field->name)) {//多层作用域，这里只找栈顶
            printf("Error type 3 at Line %d: Redefined variable %s.",temp->lineNo, item->field->name);
            deleteItem(item);
        } else {
            insert_entry(stack->top->table, item->field->name,item);//没冲突就插入当前表
        }
        if (temp->child->next) {
            temp = temp->next->next->child;
        } else {
            break;
        }
    }
}

pType Specifier(pNode node,Stack* stack) {//这个函数需要返回变量类型
    assert(node != NULL);
    // Specifier -> TYPE
    //            | StructSpecifier

    pNode t = node->child;
    // Specifier -> TYPE
    if (!strcmp(t->name, "TYPE")) {
        if (!strcmp(t->val, "float")) {
            return newType(FLOAT_TYPE,FLOAT_TYPE);
        } else {
            return newType(INT_TYPE,INT_TYPE);
        }
    }
    // Specifier -> StructSpecifier
    else {
        return StructSpecifier(t,stack);
    }
}
pType StructSpecifier(pNode node,Stack* stack) {
    assert(node != NULL);
    // StructSpecifier -> STRUCT OptTag LC DefList RC
    //                  | STRUCT Tag

    // OptTag -> ID | e
    // Tag -> ID
    pType returnType = NULL;
    pNode t = node->child->next;
    // StructSpecifier->STRUCT OptTag LC DefList RC
    // printTreeInfo(t, 0);
    if (strcmp(t->name, "Tag")) {
        // addStructLayer(table);
        pItem structItem =//这里是给结构体名字赋值
            initializeItem(initializeFieldList("", newType(STRUCT_TYPE, NULL, NULL)));
        if (!strcmp(t->name, "OptTag")) {
            setFieldListName(structItem->field, t->child->val);
            t = t->next;
        }
        // unnamed struct未命名结构体,根据实践打印的语法树可以看出，optTag为空时该节点不存在
        else {
            printf("结构体未命名");
            exit(0);
        }
        //现在我们进入结构体了！注意，报错信息会有不同！
        if (!strcmp(t->next->name, "DefList")) {
            DefList(t->next, structItem,stack);//携带了结构体信息，用于定义结构体
        }

        if (conflict(stack, structItem->field->name)) {
            printf("Error type 16 at Line:%d,name:%s",node->lineNo,structItem->field->name);
            deleteItem(structItem);
        } else {
            returnType = newType(
                STRUCT_TYPE, newString(structItem->field->name),
                copyFieldList(structItem->field->type->u.structure.field));

            // printf("\nnew Type:\n");
            // printType(returnType);
            // printf("\n");

            if (!strcmp(node->child->next->name, "OptTag")) {
                insert_entry(peek(stack), structItem->field->name,structItem);
            }
            // OptTag -> e
            else {
                deleteItem(structItem);//未命名结构体
            }
        }
}
// StructSpecifier->STRUCT Tag
    else {
        pItem structItem = stack_search_item(stack, t->child->val);
        if (structItem == NULL || !isStructDef(structItem)) {
            printf("Error type 17 at Line %d: Undefined structure %s.", t->lineNo,t->child->val);
        
        
        
        } else
            returnType = newType(
                STRUCT_TYPE, newString(structItem->field->name),
                copyFieldList(structItem->field->type->u.structure.field));
    }
    // printType(returnType);
    return returnType;
}
pItem VarDec(pNode node, pType specifier) {
    assert(node != NULL);
    // VarDec -> ID
    //         | VarDec LB INT RB
    pNode id = node;
    // get ID
    while (id->child) id = id->child;
    //printf("特别声明：%s\n",id->val);
    pItem p = initializeItem(initializeFieldList(id->val, NULL));
    if (node->child&&!strcmp(node->child->name, "ID")) {
        // printf("copy type tp %s.\n", node->child->val);
        p->field->type = copyType(specifier);
    }
    // VarDec -> VarDec LB INT RB
    else {
        pNode varDec = node->child;
        pType temp = specifier;
        // printf("VarDec -> VarDec LB INT RB.\n");
        while (varDec->next) {
            
            p->field->type =newType(ARRAY, copyType(temp), atoi(varDec->next->next->val));
            temp = p->field->type;//基本元素是上一维数组，a[10][100]第一维就是a[100]
            varDec = varDec->child;
            

        }
    }
    return p;
}


void FunDec(pNode node, pType returnType,Stack* stack) {
    assert(node != NULL);
    // FunDec -> ID LP VarList RP
    //         | ID LP RP
     //printf("\n%s\n",node->child->val);
    pItem p =
        initializeItem(initializeFieldList(node->child->val,newType(FUN, 0, NULL, copyType(returnType))));
    // if (p==NULL)
    //     printf("p是空的");
    // else
    //     printf("pno是空的");

    // if (p->field==NULL)
    //     printf("与是空的");
    // if (p->field->name==NULL)
    //     printf("字符串为空");
    // else
    //     printf("字符串存在%s",p->field->name);
    if (conflict(stack, p->field->name)) {
        //printf("函数怎么重定义了\n");
        printf( "Error type 4 at Line %d Redefined function %s.",node->lineNo, p->field->name);
        deleteItem(p);
        p = NULL;
    } else {
        insert_entry(peek(stack),p->field->name,p);//参数进一步压栈之前先把函数放到上一层
        //printf("函数插入table\n");
    }
    // FunDec -> ID LP VarList RP
    if (!strcmp(node->child->next->next->name, "VarList")) {
        VarList(node->child->next->next, p,stack);
    }
    // FunDec -> ID LP RP don't need process
    if (strcmp(node->child->next->next->name, "VarList")) {
        //push(stack,create_table());//没有参数也压栈
    }
    // check redefine
    
}

void VarList(pNode node, pItem func,Stack* stack) {
    assert(node != NULL);
    // VarList -> ParamDec COMMA VarList
    //          | ParamDec
    //这里有压栈操作，首先要初始化表格
    //push(stack,create_table());
    int argc = 0;
    pNode temp = node->child;
    pFieldList cur = NULL;

    // VarList -> ParamDec
    pFieldList paramDec = ParamDec(temp,stack);
    func->field->type->u.function.argv = copyFieldList(paramDec);
    cur = func->field->type->u.function.argv;
    argc++;

    // VarList -> ParamDec COMMA VarList
    while (temp->next) {
        temp = temp->next->next->child;
        paramDec = ParamDec(temp,stack);
        if (paramDec) {
            cur->tail = copyFieldList(paramDec);//把参数穿起来
            cur = cur->tail;
            argc++;
        }
    }
    func->field->type->u.function.argc = argc;
}
pFieldList ParamDec(pNode node,Stack* stack) {
    assert(node != NULL);
    // ParamDec -> Specifier VarDec
    pType specifierType = Specifier(node->child,stack);
    pItem p = VarDec(node->child->next, specifierType);
    if (specifierType) deleteType(specifierType);
    if (conflict(stack, p->field->name)) {
        printf("line %d Redefined variable %s.",node->lineNo, p->field->name);
        deleteItem(p);
        return NULL;
    } else {
        insert_entry(peek(stack),p->field->name,p);
        return p->field;
    }
}
void CompSt(pNode node, pType returnType,Stack* stack) {
    assert(node != NULL);
    //这里要加深栈
    //push(stack,create_table());
    // CompSt -> LC DefList StmtList RC
    // printTreeInfo(node, 0);
    pNode temp = node->child->next;
    if (!strcmp(temp->name, "DefList")) {
        DefList(temp, NULL,stack);
        temp = temp->next;
    }
    if (!strcmp(temp->name, "StmtList")) {
        StmtList(temp, returnType,stack);
    }
}

void StmtList(pNode node, pType returnType,Stack*stack) {
    // assert(node != NULL);
    // StmtList -> Stmt StmtList
    //           | e
    // printTreeInfo(node, 0);
    while (node) {
        Stmt(node->child, returnType,stack);
        node = node->child->next;
    }
}

void Stmt(pNode node, pType returnType,Stack* stack) {
    assert(node != NULL);
    // Stmt -> Exp SEMI
    //       | CompSt
    //       | RETURN Exp SEMI
    //       | IF LP Exp RP Stmt
    //       | IF LP Exp RP Stmt ELSE Stmt
    //       | WHILE LP Exp RP Stmt
    // printTreeInfo(node, 0);

    pType expType = NULL;
    // Stmt -> Exp SEMI
    if (!strcmp(node->child->name, "Exp")) expType = Exp(node->child,stack);

    // Stmt -> CompSt
    else if (!strcmp(node->child->name, "CompSt"))
        {CompSt(node->child, returnType,stack);
        //pop(stack);
        }
    // Stmt -> RETURN Exp SEMI
    else if (!strcmp(node->child->name, "RETURN")) {
        expType = Exp(node->child->next,stack);

        // check return type
        if (!checkType(returnType, expType))
           printf("Type 8 line %d 返回值类型不匹配 ",node->child->next->lineNo);
    }

    // Stmt -> IF LP Exp RP Stmt
    else if (!strcmp(node->child->name, "IF")) {
        pNode stmt = node->child->next->next->next->next;
        expType = Exp(node->child->next->next,stack);
        Stmt(stmt, returnType,stack);
        // Stmt -> IF LP Exp RP Stmt ELSE Stmt
        if (stmt->next != NULL) Stmt(stmt->next->next, returnType,stack);
    }

    // Stmt -> WHILE LP Exp RP Stmt
    else if (!strcmp(node->child->name, "WHILE")) {
        expType = Exp(node->child->next->next,stack);
        Stmt(node->child->next->next->next->next, returnType,stack);
    }

    if (expType) deleteType(expType);
}
void DefList(pNode node, pItem structInfo,Stack*stack) {
    // assert(node != NULL);
    // DefList -> Def DefList
    //          | e
    //printf("do DefList\n");
    // if (structInfo==NULL)
    // {
    //     printf("structinfo为空\n");
    // }
    while (node) {
        Def(node->child, structInfo,stack);
        node = node->child->next;
    }
}

void Def(pNode node, pItem structInfo,Stack*stack) {
    assert(node != NULL);
    // Def -> Specifier DecList SEMI
    pType dectype = Specifier(node->child,stack);

    DecList(node->child->next, dectype, structInfo,stack);
    if (dectype) deleteType(dectype);
}

void DecList(pNode node, pType specifier, pItem structInfo,Stack*stack) {
    assert(node != NULL);
    // DecList -> Dec
    //          | Dec COMMA DecList
    pNode temp = node;
    while (temp) {
        Dec(temp->child, specifier, structInfo,stack);
        if (temp->child->next)
            temp = temp->child->next->next;
        else
            break;
    }
}
void Dec(pNode node, pType specifier, pItem structInfo,Stack* stack) {
    assert(node != NULL);
    // Dec -> VarDec
    //      | VarDec ASSIGNOP Exp

    // Dec -> VarDec
    if (node->child->next == NULL) {
        if (structInfo != NULL) {
            // 结构体内，将VarDec返回的Item中的filedList
            // Copy判断是否重定义，无错则到结构体链表尾 记得delete掉Item
            pItem decitem = VarDec(node->child, specifier);
            pFieldList payload = decitem->field;
            pFieldList structField = structInfo->field->type->u.structure.field;
            pFieldList last = NULL;
            while (structField != NULL) {
                // then we have to check
                //printf("当前域名：%s\n",structField->name);
                if (!strcmp(payload->name, structField->name)) {
                    //出现重定义，报错
                    printf("Type 15  line %d Redefined field %s.",node->lineNo,
                            decitem->field->name);
                    deleteItem(decitem);
                    return;
                } else {
                    last = structField;
                    structField = structField->tail;
                }
            }
            //printf("\n议论结束\n");
            //新建一个fieldlist,删除之前的item
            if (last == NULL) {
                // that is good
                structInfo->field->type->u.structure.field =
                    copyFieldList(decitem->field);
            } else {
                last->tail = copyFieldList(decitem->field);
            }
            deleteItem(decitem);
        } else {
            // 判断返回的item有无冲突，无冲突放入表中，有冲突报错delete
            //printf("VarDec前夕\n");
            pItem decitem = VarDec(node->child, specifier);//我认为这里包含了结构体变量的定义
            if (conflict(stack, decitem->field->name)) {
                //出现冲突，报错
                printf("type3 line %d Redefined variable %s.",node->lineNo,
                        decitem->field->name);
                deleteItem(decitem);
            } else {
                insert_entry(peek(stack),decitem->field->name,decitem);
            }
        }
    }
    // Dec -> VarDec ASSIGNOP Exp
    else {
        if (structInfo != NULL) {
            // 结构体内不能赋值，报错
            //printf("结构体内不能赋值");
        } else {
            // 判断赋值类型是否相符
            //如果成功，注册该符号
            //printf("\n= VarDec前夕\n");
            pItem decitem = VarDec(node->child, specifier);
            //printf("声明之后%s\n",decitem->field->name);
            //printf("=Vardec over");
            pType exptype = Exp(node->child->next->next,stack);
            if (conflict(stack, decitem->field->name)) {
                //出现冲突，报错
                printf("type3 line %d Redefined variable %s.\n",node->lineNo,
                        decitem->field->name);
                deleteItem(decitem);
                return;
            }
            if (!checkType(decitem->field->type, exptype)) {
                //类型不相符
                //报错
                printf("\ntype5 类型不相符 kind:%d,basic:%d\n",decitem->field->type->kind,exptype->u.basic);
                deleteItem(decitem);
                return;
            }
            //printf("声明之后1%s\n",decitem->field->name);
            if (decitem->field->type && decitem->field->type->kind == ARRAY) {
                //报错，对非basic类型赋值
                deleteItem(decitem);
            } else {
                insert_entry(peek(stack),decitem->field->name,decitem);
            }
            // exp不出意外应该返回一个无用的type，删除
            if (exptype) deleteType(exptype);
        }
    }
}

pType Exp(pNode node,Stack*stack) {
    assert(node != NULL);
    // Exp -> Exp ASSIGNOP Exp
    //      | Exp AND Exp
    //      | Exp OR Exp
    //      | Exp RELOP Exp
    //      | Exp PLUS Exp
    //      | Exp MINUS Exp
    //      | Exp STAR Exp
    //      | Exp DIV Exp
    //      | LP Exp RP
    //      | MINUS Exp
    //      | NOT Exp
    //      | ID LP Args RP
    //      | ID LP RP
    //      | Exp LB Exp RB
    //      | Exp DOT ID
    //      | ID
    //      | INT
    //      | FLOAT
    pNode t = node->child;
    // exp will only check if the cal is right
    //  printTable(table);
    //二值运算
    if (!strcmp(t->name, "Exp")) {
        // 基本数学运算符
        if (strcmp(t->next->name, "LB") && strcmp(t->next->name, "DOT")) {
            pType p1 = Exp(t,stack);
            pType p2 = Exp(t->next->next,stack);
            pType returnType = NULL;

            // Exp -> Exp ASSIGNOP Exp
            if (!strcmp(t->next->name, "ASSIGNOP")) {
                //检查左值
                pNode tchild = t->child;

                if (!strcmp(tchild->name, "FLOAT") ||
                    !strcmp(tchild->name, "INT")) {
                    //报错，左值
                    //printf("左值");
                    printf("Error type 6 at line %d",node->lineNo);

                } 
                else if (!strcmp(tchild->name, "ID") ||
                           !strcmp(tchild->next->name, "LB") ||
                           !strcmp(tchild->next->name, "DOT"))
                {
                    if (!checkType(p1, p2)) {
                        //报错，类型不匹配
                        printf("Error type 5 at line %d",node->lineNo);
                    } else
                        returnType = copyType(p1);
                } else {
                    //报错，左值
                    printf("Error type 6 at line %d",node->lineNo);
                }
            }
            // Exp -> Exp AND Exp
            //      | Exp OR Exp
            //      | Exp RELOP Exp
            //      | Exp PLUS Exp
            //      | Exp MINUS Exp
            //      | Exp STAR Exp
            //      | Exp DIV Exp
            else {
                if (p1 && p2 && (p1->kind == ARRAY || p2->kind == ARRAY)) {
                    //报错，数组，结构体运算
                   //("数组结构体参与二元运算");
                } 
                else if (!checkType(p1, p2)) {
                    //报错，类型不匹配
                    //printf("类型不匹配");
                    printf("Error type 7 at line %d",node->lineNo);
                } 
                else {
                    if (p1 && p2) {
                        returnType = copyType(p1);
                    }
                }
            }

            if (p1) deleteType(p1);
            if (p2) deleteType(p2);
            return returnType;
        }
        // 数组和结构体访问
        else {
            // Exp -> Exp LB Exp RB
            if (!strcmp(t->next->name, "LB")) {
                //数组
                pType p1 = Exp(t,stack);
                pType p2 = Exp(t->next->next,stack);
                pType returnType = NULL;

                if (!p1) {
                    // 第一个exp为null，上层报错，这里不用再管
                } else if (p1 && p1->kind != ARRAY) {
                    //报错，非数组使用[]运算符
                    printf("Error type 10 at line %d",node->lineNo);
                } else if (!p2 || p2->kind != INT_TYPE ||
                           p2->u.basic != INT_TYPE) {
                    //报错，不用int索引[]

                    printf("Error type 12 at line %d: %s\" is not an integer.",node->lineNo,
                            t->next->next->child->val);
                } else {
                    returnType = copyType(p1->u.array.elem);
                }
                if (p1) deleteType(p1);
                if (p2) deleteType(p2);
                return returnType;
            }
            // Exp -> Exp DOT ID
            else {
                pType p1 = Exp(t,stack);
                pType returnType = NULL;
                if (!p1 || p1->kind != STRUCT_TYPE ||
                    !p1->u.structure.structName) {
                    //报错，对非结构体使用.运算符
                    printf("Error type 13 at Line %d: Illegal use of \".\".",t->lineNo);
                    if (p1) deleteType(p1);
                    exit(-1);
                } else {//查找域名是否存在
                    pNode ref_id = t->next->next;
                    pFieldList structfield = p1->u.structure.field;
                    while (structfield != NULL) {
                        if (!strcmp(structfield->name, ref_id->val)) {
                            break;
                        }
                        structfield = structfield->tail;
                    }
                    if (structfield == NULL) {
                        //报错，没有可以匹配的域名
                        printf("Error type 14 at Line %d: %s.\n",  t->lineNo,
                               "NONEXISTFIELD");
                    } else {
                        returnType = copyType(structfield->type);
                    }
                }
                if (p1) deleteType(p1);
                return returnType;
            }
        }
    }
    //单目运算符
    // Exp -> MINUS Exp
    //      | NOT Exp
    else if (!strcmp(t->name, "MINUS") || !strcmp(t->name, "NOT")) {
        pType p1 = Exp(t->next,stack);
        pType returnType = NULL;
        if (!p1 || (p1->kind != INT_TYPE &&p1->kind != FLOAT_TYPE)) {
            //报错，数组，结构体运算
            printf("Error type 7 at Line %d: %s.\n", t->lineNo,
                   "TYPE_MISMATCH_OP");
        } else {
            returnType = copyType(p1);
        }
        if (p1) deleteType(p1);
        return returnType;
    } 
    else if (!strcmp(t->name, "LP")) {
        return Exp(t->next,stack);
    }
    // Exp -> ID LP Args RP
    //		| ID LP RP
    else if (!strcmp(t->name, "ID") && t->next) {
        //printf("执行到检验的步骤了");
        pItem funcInfo = stack_search_item(stack, t->val);
        //printf("搜索完毕了");
        // function not find
        if (funcInfo == NULL) {
            char msg[100] = {0};
            printf("Error type 2 at Line %d: Undefined function %s.",t->lineNo,t->val);
            return NULL;
        } else if (funcInfo->field->type->kind != FUN) {
            printf("Error type 11 at Line %d: Undefined function %s.",t->lineNo,t->val);

            return NULL;
        }
        // Exp -> ID LP Args RP
        else if (!strcmp(t->next->next->name, "Args")) {
            Args(t->next->next, funcInfo,stack);
            
            return copyType(funcInfo->field->type->u.function.returnType);
        }
        // Exp -> ID LP RP
        else {
            if (funcInfo->field->type->u.function.argc != 0) {
                printf("Error type 9 at Line %d: Undefined function %s.",t->lineNo,t->val);
            }
            return copyType(funcInfo->field->type->u.function.returnType);
        }
    }
    // Exp -> ID
    else if (!strcmp(t->name, "ID")) {
        pItem tp = stack_search_item(stack, t->val);
        if (tp == NULL || isStructDef(tp)) {//也不能是函数定义
            printf("Error type 1 at Line %d:  Undefined variable %s.",t->lineNo,t->val);
            return NULL;
        } else {
            // good
            return copyType(tp->field->type);
        }
    } else {
        // Exp -> FLOAT
        if (!strcmp(t->name, "FLOAT")) {
            return newType(FLOAT_TYPE, FLOAT_TYPE);
        }
        // Exp -> INT
        else {
            return newType(INT_TYPE, INT_TYPE);
        }
    }
}

void Args(pNode node, pItem funcInfo,Stack*stack) {
    assert(node != NULL);
    // Args -> Exp COMMA Args
    //       | Exp
    // printTreeInfo(node, 0);
    pNode temp = node;
    pFieldList arg = funcInfo->field->type->u.function.argv;
    // printf("-----function atgs-------\n");
    // printFieldList(arg);
    // printf("---------end-------------\n");
    while (temp) {
        if (arg == NULL) {
            printf(
                "Error type 9 at line %d too many arguments to function \"%s\", except %d args.",node->lineNo,
                funcInfo->field->name, funcInfo->field->type->u.function.argc);

            break;
        }
        pType realType = Exp(temp->child,stack);

        if (!checkType(realType, arg->type)) {

            printf("Type 9 at line %d Function \"%s\" is not applicable for arguments.",node->lineNo,
                    funcInfo->field->name);

            if (realType) deleteType(realType);
            return;
        }
        if (realType) deleteType(realType);

        arg = arg->tail;
        if (temp->child->next) {
            temp = temp->child->next->next;
        } else {
            break;
        }
    }
    if (arg != NULL) {
        printf("Type 9 at Line %d too few arguments to function \"%s\", except %d args.",node->lineNo,
                funcInfo->field->name, funcInfo->field->type->u.function.argc);
    }
}
