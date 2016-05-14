#include "../h/Tables.h"
#include "../h/std.h"

/* returns the precedence of the given symbol. A precedence of 0 overrides
 * all other symbols, and higher precedences become less and less potent.
 * a precedence of -1 is used to indicate a symbol not in the symbol table.
 */
int evaluatePrecedence(char sym) {

    int tableSize = sizeof(_SYMBOLS)/sizeof(char*);
    for ( int i=0 ; i<tableSize ; i++ ) {
        if ( strchr(_SYMBOLS[i],sym) ) return i;
    }
    return -1;

}

/* returns index of symbol if the given string contains a symbol, -1 otherwise. */
int containsSymbol(char* str) {

    for ( int i=0 ; str[i] ; i++ ) {
        if ( -1 != evaluatePrecedence(str[i]) ) return i;
    }
    return -1;

}


/* Checks whether the symbol given to it is unary. Returns -1 if it's not a symbol,
 * 0 if it's not unary, and 1 if it is unary.
 */
int isUnary(char sym) {

    int out = evaluatePrecedence(sym);
    if ( -1 == out ) return -1;
    char* ref = strchr(_SYMBOLS[out],(int)sym);
    if ( *ref == *(ref+1) ) return 0;
    return 1;

}

/* Checks whether the given two symbols complement each other, close brackets
 * or parentheses, etc. Returns 0 if they do not complement each other, 1 if
 * they do, or -1 if either character is not a symbol.
 */
int symbolComplements(char sym1, char sym2) {

    int sym1p = evaluatePrecedence(sym1);
    int sym2p = evaluatePrecedence(sym2);
    if ( -1 == sym1p || -1 == sym2p ) return -1;
    if ( sym1p != sym2p ) return 0;

    if ( isDirectional(sym1) ) return directionalComplement(sym1,sym2);
    else if ( sym1 == sym2 ) return 1;
    return 0;

}

/* Returns 1 if the specified symbol has a directional compliment like
 * parentheses or brackets, or 0 if it does not, like quotes.
 */
int isDirectional(char sym) {

    if ( NULL != strchr("(){}[]",sym) ) return 1;
    return 0;

}

/* Returns whether the specified symbols compliment each other,
 * i.e. an open parenthesis and close parenthesis.
 */
int directionalComplement(char sym1, char sym2) {

    char* open = "({[";
    char* close = ")}]";
    if ( !isDirectional(sym1) || !isDirectional(sym2) ) return -1;
    char closeSymbol = close[strchr(open,sym1) - open];
    if ( sym2 == closeSymbol ) return 1;
    return 0;

}

/* returns 1 if the command is meant to run in the background, 0 otherwise. */
int isBackground(Token* command) {
    if ( command->value[0] == '&' && !command->right ) return 1;
    return 0;
}

/* Condensation rule for a pair of binary logical delineators
 *(quotes, brackets, etc).
 */
Token* condenseQ(Token* startQ, Token* endQ) {

    Token* head = tokenHead(startQ);

    if ( startQ->next == endQ ) {
        head = removeToken(startQ);
        head = removeToken(endQ);
        if ( !head ) return newToken();
        return head;
    }

    // these are the start and end tokens of the quote.
    Token* quoteStart = startQ->next;
    Token* quoteEnd   = endQ->prev;

    Token* placeholder = newTokenV(startQ->value);
    placeholder->prec = -1;
    head = removeToken(startQ);
    head = removeToken(endQ);

    Token* before = head;
    Token* quote = splitBefore(quoteStart);
    if ( quote == before ) before = 0;
    Token* after = splitAfter(quoteEnd);
    if ( after == tokenTail(quote) ) after = 0;

    placeholder->down = quote;
    quote->up = placeholder;
    Token* ret;
    ret = appendTokenLL(before,placeholder);
    ret = appendTokenLL(placeholder,after);
    
    return tokenHead(ret);

}

/* Condensation rule for a unary delineator (&, |, etc). */
Token* condenseP(Token* sym) {

    Token* right = sym->next;
    Token* left = (sym->prev) ? tokenHead(sym->prev) : (Token*)0;

    if ( -1 != (int)sym->prec && sym->value[0] == '\\' ) {
        if ( !right ) {
            ERROR_MISSING_ARGUMENT(newTokenV("\\"));
            return (Token*)0;
        }
        splitBefore(sym);
        splitAfter(sym);
        if ( !(strlen(right->value) == 1) ) {
            insertAfter(right,newTokenV(right->value+1));
            char* temp = safe_malloc(2*sizeof(char));
            temp[0] = right->value[0];
            temp[1] = 0;
            free(right->value);
            right->value = temp;
        }
        Token* rightright = right->next;
        splitAfter(right);
        sym->down = right;
        right->up = sym;
        if ( left ) appendTokenLL(left,sym);
        if ( rightright ) appendTokenLL(sym,rightright);
        sym->prec = -1;
        return tokenHead(sym);
    }

    stealUpToken(sym);
    splitBefore(sym);
    splitAfter(sym);
    sym->left = left;
    if ( left ) left->up = sym;
    sym->right = right;
    if ( right ) right->up = sym;
    sym->prec = -1;
    return tokenHead(sym);

}

/* purely a helper function, goes through the next-prev neighbors of
 * the given Token Linked List, and if any of them are linked to a node
 * upwards, unlinks that one and links the Token self instead. Returns
 * 1 on a successful steal, or 0 if there was no up Token to be stolen.
 */
int stealUpToken(Token* self) {

    Token* cur = tokenHead(self);
    while ( cur ) {
        if ( cur->up ) break;
        cur = cur->next;
    }
    if ( !cur ) return 0;
    if ( cur == cur->up->down ) cur->up->down = self;
    if ( cur == cur->up->left ) cur->up->left = self;
    if ( cur == cur->up->right ) cur->up->right = self;
    self->up = cur->up;
    cur->up = 0;
    return 1;

}







