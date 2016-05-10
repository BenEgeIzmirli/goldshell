#include "../h/std.h"
#include "../h/Tokenizer.h"
#include "../c/TokenUtils.c"
#include "../c/Tables.c"

/* Takes an input line, and creates a Token Linked List.
 * Delineates using isspace. Each Token contains a deep
 * copy of the original word.
 */
Token* strToToken(char* str) {

    if ( strlen(str) <= 1 ) return newToken();

    char* strTemp = myStrndup(str);

    Token* head = newToken();
    Token* cur = head;

    int FSM = 0; // 0 = reading spaces ; 1 = reading a word
    for ( char* c=strTemp ; *c ; c++ ) {
        if ( isspace(*c) ) {
            FSM = 0;
            *c = '\0';
            continue;
        }
        if ( !FSM ) { // if previous character was a space
            FSM = 1;
            if ( cur->value ) cur = appendToken(cur,newToken());
            cur->value = c;
        }
    }

    if ( !head->value ) {
        free(strTemp);
        return head;
    }

    for ( Token* t=head ; t ; t = t->next ) {
        t->value = myStrndup(t->value);
    }

    free(strTemp);

    return head;

}


Token* symbolSplit(Token* head) {

    while ( head ) {
        if ( strlen(head->value) == 1 ){
            head = head->next;
            continue;
        }
        for ( char* c=head->value ; *c ; c++ ) {
            
        }
    }

    return newToken();

}













