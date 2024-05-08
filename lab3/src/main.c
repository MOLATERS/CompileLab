#include "parser.h"
#include "sematic.h"
#include "stdio.h"
#include "stdlib.h"
#include "inter.h"

int synError = 0;
int lexError = 0;
int interError = 0;

extern Stk stack;
extern pInterCodeList interCodeList;
extern TNode* root;
extern int yydebug;
extern int yyparse();
extern void yyrestart(FILE*);


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

    // 构建语法树
    yyrestart(f);
    yyparse();

    // 进行语义分析
    stack = createStack();
    stack = initStack();
    stack = Push(initTable());
    Item read_func = newItem("read",newType(FUNC_TYPE, 0, NULL, copyType(INT_TYPE)));
    Item a_int = newItem("a", newType(INT_TYPE, INT_TYPE));
    Item write_func = newItem("write", newType(FUNC_TYPE, 1, a_int, copyType(INT_TYPE)));
    insertItem(stack->top->table, read_func->field->name, read_func);
    insertItem(stack->top->table, a_int->field->name, a_int);
    insertItem(stack->top->table, write_func->field->name, write_func);
    TreeScan(root->child);

    // 执行中间代码生成
    genInterCodes(root);

    fclose(f);
    return 0;
}
