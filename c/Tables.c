#include "../h/Tables.h"
#include "../h/std.h"

int evaluatePrecedence(char sym) {

    int tableSize = sizeof(_SYMBOLS)/sizeof(char*);
    for ( int i=0 ; i<tableSize ; i++ ) {
        if ( strchr(_SYMBOLS[i],sym) ) return i;
    }
    return -1;

}





