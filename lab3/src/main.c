#include "parser.h"
#include "sematic.h"
#include "inter.h"
#include "stdio.h"

int synError = 0;
int lexError = 0;

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
    yyrestart(f);
    yyparse();

    // if (!lexError && !synError) {
    //     PreOrder(root, 0);
    // }

    Stk stack = createStack();
    Push(stack, initTable());
    TreeScan(root, stack);
    return 0;
}
