#include "../h/std.h"
#include "../h/TokenUtils.h"

/* Creates a new Token object, initialized to 0. */
Token* newToken() {

    Token* ret = (Token*) safe_malloc(sizeof(Token));
    memset(ret, 0, sizeof(Token));
    return ret;

}

/* Creates a new Token object, initialized as specified. */
Token* newTokenV(char* value) {

    Token* ret = newToken();
    ret->value = myStrndup(value);
    return ret;

}

/* Prints a Token object and all its relevant information. */
void printToken(Token* t) {

    printf("----- Token at %p -----\n",(void*)t);
    printf("| value:    %s\n",t->value);
    printf("| verb:     %u\n",t->verb);
    printf("| symbol:   %u\n",t->symbol);
    printf("| argument: %u\n",t->argument);
    printf("| next:     %p\n",(void*)t->next);
    printf("| prev:     %p\n",(void*)t->prev);
    printf("----- END TOKEN -----\n");

}


/* Appends the Token t to the end of the Token
 * Linked List starting with head.
 */
Token* appendToken(Token* head, Token* t) {

    while ( head->next ) head = head->next;
    head->next = t;
    t->prev = head;
    t->next = 0;

    return t;

}

/* Inserts a Token t before the Token pointed to by after.
 * Returns the new head of the Token Linked List.
 * NOT YET TESTED.
 */
Token* insertToken(Token* after, Token* t) {

    Token* left = after->prev;
    Token* right = after;
    if ( left ) left->next = t;
    t->prev = left;
    t->next = right;
    right->prev = t;

    while ( after->prev ) after = after->prev;
    return after;

}

/* Prints a Linked List of Tokens from the start of the list. */
void printTokenLL(Token* head) {

    while ( head->prev ) head = head->prev;
    while ( head ) {
        printToken(head);
        head = head->next;
    }

}


/* Frees allocated memory for a Token Linked List from the
 * start of the list.
 */
void freeTokenLL(Token* head) {

    while ( head->prev ) head = head->prev;

    Token* next;
    while ( head ) {
        next = head->next;
        free(head->value);
        free(head);
        head = next;
    }

}



