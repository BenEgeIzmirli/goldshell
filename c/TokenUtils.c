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

    if ( !value ) return newToken();
    Token* ret = newToken();
    ret->value = myStrndup(value);
    return ret;

}

/* Prints a Token object and all its relevant information. */
void printToken(Token* t) {

    printf("----- Token at %p -----\n",(void*)t);
    printf("| value:    %s\n",t->value);
    printf("| next:     %p\n",(void*)t->next);
    printf("| prev:     %p\n",(void*)t->prev);
    printf("----- END TOKEN -----\n");

}


/* Copies a Token into a new Token object. Returns a pointer to the new Token. */
Token* copyToken(Token* t) {

    Token* ret = newTokenV(t->value);
    ret->next = t->next;
    ret->prev = t->prev;
    return ret;

}

/* Copies a Token Linked List, and returns a pointer to the head of the new list. */
Token* copyTokenLL(Token* head) {

    head = tokenHead(head);
    Token* ret = newTokenV(head->value);
    head = head->next;

    while ( head ) {
        ret = appendToken(ret,copyToken(head));
        head = head->next;
    }

    return tokenHead(ret);

}


/* Appends the Token t to the end of the Token
 * Linked List starting with head. Returns the head of
 * the Linked List.
 */
Token* appendToken(Token* head, Token* t) {

    while ( head->next ) head = head->next;
    head->next = t;
    t->prev = head;
    t->next = 0;

    return tokenHead(t);

}

/* returns a pointer to the head of a Token Linked List */
Token* tokenHead(Token* t) {

    while ( t->prev ) t = t->prev;
    return t;

}

/* returns a pointer to the last element of a Token Linked List */
Token* tokenTail(Token* t) {

    while ( t->next ) t = t->next;
    return t;

}


/* Inserts a Token t before the Token pointed to by after.
 * Returns the new head of the Token Linked List.
 * NOT YET TESTED.
 */
Token* insertBefore(Token* after, Token* t) {

    Token* left = after->prev;
    Token* right = after;
    if ( left ) left->next = t;
    t->prev = left;
    t->next = right;
    right->prev = t;

    return tokenHead(after);

}


/* Inserts a Token t after the Token pointed to by before.
 * Returns the new head of the Token Linked List.
 * NOT YET TESTED.
 */
Token* insertAfter(Token* before, Token* t) {

    Token* left = before;
    Token* right = before->next;
    if ( right ) right->prev = t;
    t->prev = left;
    t->next = right;
    left->next = t;

    return tokenHead(before);

}

/* removes a Token from the Linked List it's in. Returns the new head of the LL. */
Token* removeToken(Token* t) {

    Token* left = t->prev;
    Token* right = t->next;
    if ( left ) left->next = right;
    if ( right ) right->prev = left;

    freeToken(t);

    if ( left ) return left;
    if ( right ) return right;
    return 0;

}

/* Prints a Linked List of Tokens from the start of the list. */
void printTokenLL(Token* head) {

    while ( head->prev ) head = head->prev;
    while ( head ) {
        printToken(head);
        head = head->next;
    }

}

/* Frees allocated memory for a Token object. */
void freeToken(Token* t) {

    if ( t->value ) free(t->value);
    free(t);

}

/* Frees allocated memory for a Token Linked List from the
 * start of the list.
 */
void freeTokenLL(Token* head) {

    while ( head->prev ) head = head->prev;

    Token* next;
    while ( head ) {
        next = head->next;
        freeToken(head);
        head = next;
    }

}



