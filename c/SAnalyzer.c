#include "../h/std.h"
#include "../h/SAnalyzer.h"


/* Takes a Token Linked List, and evaluates the precedence for each Token. */
Token* setPrecedences(Token* head) {

    if ( !head->value ) return head;
    Token* cur = tokenHead(head);
    while ( cur ) {
        cur->prec = evaluatePrecedence(cur->value[0]);
        cur->symbol = cur->prec;
        cur = cur->next;
    }
    return tokenHead(head);

}

Token* setTreeSymbols(Token* head) {

    if ( !head ) return (Token*)0;
    if ( !head->value ) return head;
    Token* highest = head;
    for ( Token* cur=head ; cur ; cur = cur->next ) {
        cur->symbol = evaluatePrecedence(cur->value[0]);
        if ( highest->symbol > cur->symbol ) highest=cur;
        if ( cur->down ) setTreeSymbols(cur->down);
        if ( cur->left ) setTreeSymbols(cur->left);
        if ( cur->right ) setTreeSymbols(cur->right);
    }
    int leaf = (highest->symbol == (unsigned int)-1) ? 1 : 0;
    for ( Token* cur=head ; cur ; cur = cur->next ) {
        cur->leaf = leaf;
    }

    return head;

}

/* Finds and returns first Token in the Token Linked List
 * with the highest precedence, starting from the Token given.
 * i.e. if your tokens has precedences -1, -1, 2, 5, 2, -1
 * then calling it from the 0th node would yield the first
 * precedence-2 node (since higher values indicate a lower
 * precedence). Also, if the highest-precedence token is
 * unary, it returns the last one, bc order of operations.
 */
Token* findHighestPrecedence(Token* head) {

    Token* highest = head;
    Token* lastHighest = head;
    while ( head ) {
        if ( highest->prec > head->prec ) highest = head;
        if ( highest->prec >= head->prec ) lastHighest = head;
        head = head->next;
    }

    if ( isUnary(highest->value[0]) ) return lastHighest;
    return highest;

}

/* Finds the complement of the given symbol Token in the remainder of
 * the Token Linked List.
 */
Token* findComplement(Token* start) {

    char startSymbol = start->value[0];
    char curSymbol;
    Token* cur = start->next;
    if ( !cur ) ERROR_NO_MATCHING_SYMBOL(start);
    while ( cur ) {
        curSymbol = cur->value[0];
        int complements = symbolComplements(startSymbol,curSymbol);
        if ( 1 == complements ) return cur;
        cur = cur->next;
    }
    ERROR_NO_MATCHING_SYMBOL(start);
    return (Token*)0;

}

/* Refactors the tier (Linked List specified by next/prev) given
 * to it, i.e. moves things down, left, or right in the tree
 * appropriately. Returns the head of the tier refactored.
 */
Token* refactorTier(Token* head) {

    head = tokenHead(head);
    Token* copy = copyTokenLL(head);
    Token* sym1 = findHighestPrecedence(head);
    while ( -1 != (int)sym1->prec ) {
        char symChar = sym1->value[0];
        if ( isUnary(symChar) ) {
            head = condenseP(sym1);
        } else {
            Token* sym2 = findComplement(sym1);
            if ( ERROR_CODE ) break;
            head = condenseQ(sym1,sym2);
        }
        sym1 = findHighestPrecedence(head);
    }
    freeTokenLL(copy);

    if ( ERROR_CODE ) {
        freeTokenLL(head);
        return (Token*)0;
    }

    Token* cur = tokenHead(head);
    while ( cur ) {
        if ( cur->down ) refactorTier( cur->down );
        if ( ERROR_CODE ) return (Token*)0;
        if ( cur->left ) refactorTier( cur->left );
        if ( ERROR_CODE ) return (Token*)0;
        if ( cur->right) refactorTier( cur->right);
        if ( ERROR_CODE ) return (Token*)0;
        cur = cur->next;
    }

    return tokenHead(head);

}














