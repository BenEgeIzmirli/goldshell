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
    return _UNARY[out];

}



