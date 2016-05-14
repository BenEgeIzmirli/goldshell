#include "../h/std.h"
#include "../h/Tokenizer.h"
#include "../c/TokenUtils.c"
#include "../c/Tables.c"

/* Takes an input line, and creates a Token Linked List.
 * Delineates using isspace. Each Token contains a deep
 * copy of the original word.
 */
Token* strToToken(char* str) {

    int strLength = (strlen(str)>MAX_LINE_LENGTH) ? MAX_LINE_LENGTH : strlen(str);
    if ( strLength <= 1 ) return newToken();

    // make a duplicate string, and replace all spaces with null bytes
    // iff it's not inside a quote
    int FSM = 0; // 0=not in quote, 1=inside single quote, 2=inside double quote
    char* strTemp = myStrndup(str);
    for ( char* c=strTemp ; *c ; c++ ) {
        switch ( *c ) {
            case '\'' :
                if ( 2 == FSM ) break;
                FSM = (FSM) ? 0 : 1 ;
                //*c = '\0';
                break;
            case '"'  :
                if ( 1 == FSM ) break;
                FSM = (FSM) ? 0 : 2 ;
                //*c = '\0';
                break;
        }
        if ( !FSM && isspace(*c) ) *c = '\0';
    }
    if ( FSM ) {
        Token* t = ( 1 == FSM ) ? newTokenV("'") : newTokenV("\"") ;
        ERROR_NO_MATCHING_SYMBOL(t);
        freeToken(t);
        free(strTemp);
        return (Token*)0;
    }

    // go through duplicate string, and make a Token for each word
    Token* cur = newToken();
    char prevChar = 0;
    for ( int i=0 ; i<strLength ; prevChar = strTemp[i++] ) {
        if ( !prevChar && strTemp[i] ) {
            if ( !cur->value ) cur->value = myStrndup(&strTemp[i]);
            else cur = appendToken(cur,newTokenV(&strTemp[i]));
        }
    }

    free(strTemp);
    return tokenHead(cur);

}

/* Takes the given Token and analyzes whether it contains a
 * symbol. If it does, will replace the Token object with
 * Token objects corresponding to the text before the symbol,
 * the symbol itself, and the text after the symbol respectively.
 * Returns the first of the 2-3 replaced elements.
 */
Token* tokenSplit(Token* t) {

    if ( -1 == containsSymbol(t->value) ) return t;

    int len = strlen(t->value);
    char strBefore[len];
    char strSymbol[2];
    char strAfter[len];
    memset( strBefore, 0, len*sizeof(char) );
    memset( strSymbol, 0, 2*sizeof(char) );
    memset( strAfter, 0, len*sizeof(char) );

    int ind = containsSymbol(t->value);
    if ( ind ) strncpy(strBefore,t->value,ind);
    strncpy(strSymbol,t->value+ind,1);
    if ( ind < len-1 ) strncpy(strAfter,t->value+ind+1,len-ind-1);
    if ( strlen(strAfter) ) insertAfter(t,newTokenV(strAfter));
    insertAfter(t,newTokenV(strSymbol));
    if ( strlen(strBefore) ) insertAfter(t,newTokenV(strBefore));
    Token* ret = t->next;
    removeToken(t);

    return ret;

}











