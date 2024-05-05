#ifndef SEMATIC_METHODS
#define SEMATIC_METHODS
#include "parser.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 100


extern int yylineno;

typedef enum {
  INT_TYPE,
  FLOAT_TYPE,
  STRUCT_TYPE,
  ARRAY_TYPE,
  FUNC_TYPE
} TypeName;

typedef struct type {
  TypeName kind;
  union {
    TypeName basic_type;
    struct {
      struct type *type;
      int size;
    } array;
    struct {
      char *name;
      struct field *field;
    } stuc;
    struct {
      int argc;
      struct type *argv;
      struct type *type;
    } func;
  } detail;
} Type, *TypePointer;

typedef struct field {
  char *name;
  struct type *type;
  struct field *next;
} FieldList, *Field;

typedef struct handle {
  struct field *field;
} FieldHandle, *Item;

typedef struct entry {
  FieldHandle *value;
  struct entry *next;
} Entry, *EntryPointer;

typedef struct table {
  Entry *entries[TABLE_SIZE];
} EntryTable, *Table;

typedef struct stacknode {
  struct table *table;
  struct stacknode *next;
} StackNode, *SNode;

typedef struct stack {
  SNode top;
} Stack, *Stk;

// Stack -> StackNode -> EntryTable -> Entry -> FieldHandle -> FieldList ->
// Type(<-> FieldList) -> TypeName

TypePointer copyType(TypePointer type);
void deleteType(TypePointer type);

Field initFieldList(char *name, TypePointer type);
Field copyFieldList(Field field);
void deleteFieldList(Field field);
void nameFieldList(Field field, char *name);

Item initItem(Field field);
void deleteItem(Item item);

Table initTable();

unsigned int Hash(char *name);
void insertItem(Table table, char *key, Item value);
Item EntrySearch(Table table, char *key);

Stk createStack();
void initStack(Stk stack);
void Push(Stk stack, Table table);
Table Pop(Stk stack);
Table Top(Stk stack);
void deleteStack(Stk stack);

Item stack_search_item(Stack *stack, char *name);
int conflict(Stack *stack, char *name);
unsigned int get_hash_code(char *name);
#endif
