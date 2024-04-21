#include "methods.h"

TypePointer copyType(TypePointer type)
{
    if(type==NULL) return NULL;
    TypePointer newType = (TypePointer)malloc(sizeof(Type));
    assert(newType != NULL);
    newType->kind = type->kind;
    switch (newType->kind)
    {
    case INT_TYPE:
    case FLOAT_TYPE:
        newType->detail.basic_type = type->detail.basic_type;
        return newType;
    case ARRAY_TYPE:
        newType->detail.array.type = copyType(type->detail.array.type);
        newType->detail.array.size = type->detail.array.size;
        return newType;
    case STRUCT_TYPE:
        newType->detail.stuc.field = copyFieldList(type->detail.stuc.field);
        newType->detail.stuc.name = strdup(type->detail.stuc.name);
        assert(newType->detail.stuc.name != NULL);
        return newType;
    case FUNC_TYPE:
        newType->detail.func.argc = type->detail.func.argc;
        newType->detail.func.argv = copyFieldList(type->detail.stuc.field);
        newType->detail.func.type = copyType(type->detail.array.type);
        return newType;
    }
}

void deleteType(TypePointer type)
{
    if (type == NULL)
        return;
    TypeName name = type->kind;
    switch (name)
    {
    case INT_TYPE:
    case FLOAT_TYPE:
        break;
    case STRUCT_TYPE:
        deleteFieldList(type->detail.stuc.field);
        free(type->detail.stuc.name);
        break;
    case ARRAY_TYPE:
        deleteType(type->detail.array.type);
        break;
    case FUNC_TYPE:
        deleteFieldList(type->detail.func.argv);
        deleteType(type->detail.func.type);
        break;
    }
    free(type);
    return;
}

Field initFieldList(char *name, TypePointer type)
{
    Field newList = (Field)malloc(sizeof(FieldList));
    assert(newList != NULL);
    newList->name = strdup(name);
    assert(newList->name != NULL);
    newList->type = type;
    newList->next = NULL;
    return newList;
}

Field copyFieldList(Field field)
{
    assert(field != NULL);
    Field head, current;
    Field temp = field;
    head = initFieldList(field->name, copyType(temp->type));
    current = head;
    temp = temp->next;
    while (temp)
    {
        current->next = initFieldList(temp->name, copyType(temp->type));
        current = current->next;
        temp = temp->next;
    }
    return head;
}

void deleteFieldList(Field field)
{
    if (field == NULL)
        return;
    deleteFieldList(field->next);
    free(field->name);
    deleteType(field->type);
    free(field);
}

void nameFieldList(Field field, char *name)
{
    assert(field != NULL && name != NULL);
    if (field->name != NULL)
        free(field->name);
    field->name = strdup(name);
    return;
}

Item initItem(Field field)
{
    Item newItem = (Item)malloc(sizeof(FieldHandle));
    assert(newItem != NULL);
    newItem->field = field;
    return newItem;
}

void deleteItem(Item item)
{
    assert(item != NULL);
    if (item->field != NULL)
        deleteFieldList(item->field);
    free(item);
    return;
}

Table initTable()
{
    Table table = (Table)malloc(sizeof(EntryTable));
    assert(table != NULL);
    for (int i = 0; i < TABLE_SIZE; i++)
        table->entries[i] = NULL;
    return table;
}

unsigned int Hash(char *name)
{
    unsigned int val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val % 100;
}

void insertItem(Table table, char *key, Item value)
{
    unsigned int index = Hash(key);
    EntryPointer temp = (EntryPointer)malloc(sizeof(Entry));
    temp->value = value;
    temp->next = NULL;
    if (table->entries[index] == NULL)
    {
        table->entries[index] = temp;
    }
    else
    {
        Entry *current = table->entries[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = temp;
    }
}

Item EntrySearch(Table table, char *key)
{
    unsigned int index = get_hash_code(key);
    Entry *current = table->entries[index];
    while (current != NULL)
    {
        if (strcmp(current->value->field->name, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

Stk createStack(){
    Stk stack = (Stk)malloc(sizeof(Stack));
    assert(stack != NULL);
    initStack(stack);
    return stack;
}

void initStack(Stk stack)
{
    stack->top == NULL;
    return;
}

void Push(Stk stack, Table table)
{
    SNode newNode = (SNode)malloc(sizeof(Stack));
    assert(newNode != NULL);
    newNode->table = table;
    newNode->next = stack->top;
    stack->top = newNode;
    return;
}

Table Pop(Stk stack)
{
    if (stack->top == NULL)
        return NULL;
    SNode temp = stack->top;
    stack->top = temp->next;
    Table data = temp->table;
    free(temp);
    return data;
}

Table Top(Stk stack)
{
    assert(stack->top != NULL);
    return stack->top->table;
}

void deleteStack(Stk stack)
{
    while (stack->top != NULL)
    {
        Pop(stack);
    }
    return;
}

int conflict(Stack *stack, char *name)
{
    Table p = (Table)malloc(sizeof(EntryTable));
    if (p == NULL)
    {
        // printf("内存分配失败");
        return -1;
    }
    // printf("冲突校验1");
    p = Top(stack);
    if (p != NULL)
        //  printf("栈顶有元素");
        if (EntrySearch(p, name) == NULL)
        {
            // printf("\nconflict表明未发生重定义\n");
            return 0;
        }
    // printf("重定义了\n");
    return 1;
}

Item stack_search_item(Stack *stack, char *name)
{
    SNode current = stack->top;
    while (current != NULL)
    {
        Item item = EntrySearch(current->table, name);
        if (item != NULL)
        {
            return item; // 找到了匹配的item
        }
        current = current->next; // 继续向下搜索
    }
    return NULL; // 没有找到匹配的item
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
