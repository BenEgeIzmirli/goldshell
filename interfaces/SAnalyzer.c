#include "../h/std.h"
#include "../c/SAnalyzer.c"

STree* constructSTree(Token* head) {

    STree* copy = tokenToSTree(head);

    printSTreeLL(copy);
    freeSTreeLL(copy);

}

