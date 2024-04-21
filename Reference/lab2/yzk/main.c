#include "node.h"
#include "syntax.tab.h"
#include "sym.h"
extern pNode root;

extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);

unsigned lexError = FALSE;
unsigned synError = FALSE;
//int argc, char** argv
int main() {
    // if (argc <= 1) {
    //     yyparse();
    //     return 1;
    // }

    // FILE* f = fopen(argv[1], "r");
    // if (!f) {
    //     perror(argv[1]);
    //     return 1;
    // }
        FILE* f = fopen("1.cmm", "r");

    yyrestart(f);
    yyparse();

    if (!lexError && !synError) {
        printf("\n");
        printTreeInfo(root, 0);
    }
    printTreeInfo(root, 0);
    Stack* stack = create_stack();
    push(stack,create_table());
    traverseTree(root,stack);
    
    delNode(root);
    return 0;
}

