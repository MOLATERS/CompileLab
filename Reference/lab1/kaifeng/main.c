
#include "tree.h"
#include "syntax.tab.h"
#include "stdio.h"
extern treeNode root;
extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);

int lexError = 0;
int synError = 0;

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

    yyrestart(f);
    yyparse();
    if (!lexError && !synError) {
        preorder(root, 0);
    }
    release(root);
    return 0;
}
