#include "../h/std.h"
#include "../h/TokenUtils.h"

/* Creates a new Token object, initialized to 0. */
Token* newToken() {

    Token* ret = (Token*) safe_malloc(sizeof(Token));
    memset(ret, 0, sizeof(Token));
    ret->prec = -1;
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

    printTokenPrepend(t,"");

}

/* Prints a Token object, prepending the given string to each line. */
void printTokenPrepend(Token* t, char* p) {

    printf("%s----- Token at %p -----\n",p,(void*)t);
    printf("%s| value:      %s\n",p,t->value);
    printf("%s| precedence: %d\n",p,t->prec);
    printf("%s| symbol:     %d\n",p,t->symbol);
    printf("%s| leaf:       %d\n",p,t->leaf);
    printf("%s| next:       %p\n",p,(void*)t->next);
    printf("%s| prev:       %p\n",p,(void*)t->prev);
    printf("%s| up:         %p\n",p,(void*)t->up);
    printf("%s| down:       %p\n",p,(void*)t->down);
    printf("%s| left:       %p\n",p,(void*)t->left);
    printf("%s| right:      %p\n",p,(void*)t->right);
    printf("%s----- END TOKEN -----\n",p);

}

void printTokenLite(Token* t, char* p) {

    printf("%sTOKEN:%s\n",p,t->value);

}

void printTokenValues(Token* head) {

    printTokenValues_helper(head, 0);

}

void printTokenValues_helper(Token* head, int prepend) {

    while ( head->prev ) head = head->prev;
    char p[3*prepend+1];
    memset(p,' ',3*prepend);
    p[3*prepend] = 0;
    while ( head ) {
        printTokenLite(head,p);
        if ( head->down ) {
            printf("%sDOWN:\n",p);
            printTokenValues_helper(head->down,prepend+1);
        }
        if ( head->left ) {
            printf("%sLEFT:\n",p);
            printTokenValues_helper(head->left,prepend+1);
        }
        if ( head->right ) {
            printf("%sRIGHT:\n",p);
            printTokenValues_helper(head->right,prepend+1);
        }
        head = head->next;
    }

}


/* Prints a Linked List of Tokens from the start of the list. */
void printTokenLL(Token* head) {

    printTokenLL_helper(head, 0);

}

void printTokenLL_helper(Token* head, int prepend) {
 
    if ( !head ) return;
    while ( head->prev ) head = head->prev;
    char p[3*prepend+1];
    memset(p,' ',3*prepend);
    p[3*prepend] = 0;
    while ( head ) {
        printTokenPrepend(head,p);
        if ( head->down ) {
            printf("%sDOWN:\n",p);
            printTokenLL_helper(head->down,prepend+1);
        }
        if ( head->left ) {
            printf("%sLEFT:\n",p);
            printTokenLL_helper(head->left,prepend+1);
        }
        if ( head->right ) {
            printf("%sRIGHT:\n",p);
            printTokenLL_helper(head->right,prepend+1);
        }
        head = head->next;
    }

}


/* Copies a Token into a new Token object. Returns a pointer to the new Token. */
Token* copyToken(Token* t) {

    Token* ret = newTokenV(t->value);
    ret->symbol = t->symbol;
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

Token* copyTokenTree(Token* head) {

    if ( !head ) return (Token*)0;
    Token* copyHead = copyTokenLL(head);
    Token* copyCur = copyHead;
    Token* headCur = head;
    while ( copyCur && headCur ) {
        copyCur->prec = headCur->prec;
        copyCur->symbol = headCur->symbol;
        copyCur->leaf = headCur->leaf;
        copyCur->down  = copyTokenTree(headCur->down );
        if ( copyCur->down ) copyCur->down->up = copyCur;
        copyCur->left  = copyTokenTree(headCur->left );
        if ( copyCur->left ) copyCur->left->up = copyCur;
        copyCur->right = copyTokenTree(headCur->right);
        if ( copyCur->right ) copyCur->right->up = copyCur;
        copyCur = copyCur->next;
        headCur = headCur->next;
    }

    return copyHead;

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

/* Splits a Token Linked List right before the node specified.
 * Returns a pointer to the head of the second list.
 */
Token* splitBefore(Token* t) {

    if ( !t->prev ) return t;
    t->prev->next = 0;
    t->prev = 0;
    return t;

}

/* Splits a Token Linked List right after the node specified.
 * Returns a pointer to the head of the second list.
 */
Token* splitAfter(Token* t) {

    if ( !t->next ) return 0;
    Token* temp = t->next;
    t->next->prev = 0;
    t->next = 0;
    return temp;

}

/* Appends one Token Linked List to the end of another.
 * Returns the head of the new list.
 */
Token* appendTokenLL(Token* dest, Token* src) {

    if ( !src ) return dest;
    if ( !dest ) return src;
    Token* destTail = tokenTail(dest);
    destTail->next = src;
    src->prev = destTail;
    return tokenHead(src);

}


/* removes a Token from the Linked List it's in. Returns the new head of the LL. */
Token* removeToken(Token* t) {

    Token* left = t->prev;
    Token* right = t->next;
    if ( left ) left->next = right;
    if ( right ) right->prev = left;

    freeToken(t);

    if ( left ) return tokenHead(left);
    if ( right ) return tokenHead(right);
    return (Token*)0;

}

Token* removeTokenNotCommand(Token* t) {

    Token* left = t->prev;
    Token* right = t->next;
    if ( left ) left->next = right;
    if ( right ) right->prev = left;

    freeTokenNotCommand(t);

    if ( left ) return tokenHead(left);
    if ( right ) return tokenHead(right);
    return (Token*)0;

}


void freeTokenNotCommand(Token* t) {

    if ( !t ) return;
    if ( t->value ) free(t->value);
    if ( t->proc ) freeProcessNotCommand(t->proc);
    free(t);

}

/* Frees allocated memory for a Token object. */
void freeToken(Token* t) {

    if ( !t ) return;
    if ( t->value ) free(t->value);
    if ( t->proc ) freeProcess(t->proc);
    free(t);

}

/* Frees allocated memory for a Token Linked List from the
 * start of the list. Returns a null Token pointer.
 */
void freeTokenLL(Token* head) {

    if ( !head ) return;

    while ( head->prev ) head = head->prev;

    Token* next;
    while ( head ) {
        next = head->next;
        if ( head->down ) freeTokenLL(head->down);
        if ( head->left ) freeTokenLL(head->left);
        if ( head->right ) freeTokenLL(head->right);
        freeToken(head);
        head = next;
    }

    return ;

}



