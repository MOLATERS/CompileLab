#include "inter_code.h"
#include "syntax.tab.h"
extern pNode root;

extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);

unsigned lexError = FALSE;
unsigned synError = FALSE;

int main(int argc,char** argv) {
    if (argc <= 1) {
        yyparse();
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }

    FILE* fw = fopen("res.txt", "wt+");
    if (!fw) {
        perror(argv[2]);
        return 1;
    }

    yyrestart(f);
    yyparse();


    // if (!lexError && !synError) {
    //     printf("\n");
    //     printTreeInfo(root, 0);
    // }
    printTreeInfo(root, 0);
    Stack* stack = create_stack();
    push(stack,create_table());
    pItem p1 = initializeItem(initializeFieldList("read",newType(FUN, 0, NULL, copyType(INT_TYPE))));
    pItem p2 = initializeItem(initializeFieldList("a",newType(INT_TYPE,INT_TYPE)));
    pItem p3 = initializeItem(initializeFieldList("write",newType(FUN, 1, p2, copyType(INT_TYPE))));
    insert_entry(stack->top->table, p1->field->name,p1);
    insert_entry(stack->top->table, p3->field->name,p3);
    Extlist(root->child,stack);
        // printTable(table);
        interCodeList = newInterCodeList();
        genInterCodes(root,stack);
        if (!interError) {
            //printInterCode(NULL, interCodeList);
            printInterCode(fw, interCodeList);
        }
    delNode(root);
    return 0;
}

