#include "../h/std.h"
#include "../c/SAnalyzer.c"

Token* constructSTree(Token* head) {

    if ( !head ) return newToken();
    if ( !head->value ) return head;
    Token* copy = copyTokenLL(head);
    setPrecedences(copy);
    copy = refactorTier(copy);

    if ( ERROR_CODE ) {
        freeTokenLL(copy);
        return (Token*)0;
    }

    freeTokenLL(head);
    setTreeSymbols(copy);
    return tokenHead(copy);
}

