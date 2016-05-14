#include "../h/std.h"
#include "../h/SAnalyzer.h"


void printSTree(STree* head) {

    printf("----- STree at %p -----\n",(void*)head);
    printf("| token value: %s\n",head->value->value);
    printf("| precedence:  %d\n",head->prec);
    printf("| prev addr:   %p\n",(void*)head->prev);
    printf("| next addr:   %p\n",(void*)head->next);
    printf("| left addr:   %p\n",(void*)head->left);
    printf("| right addr:  %p\n",(void*)head->right);
    printf("----- END STREE -----\n");

    if ( head->left ) printSTree(head->left);
    if ( head->right ) printSTree(head->right);

}

/* Makes a new STree and returns a pointer to it. */
STree* newSTree(Token* t) {

    STree* ret = (STree*) safe_malloc(sizeof(STree));
    ret->value = t;
    ret->prec = -1;

    return ret;

}


/* Appends the STree t to the end of the STree
 * Linked List starting with head. Returns the head of
 * the Linked List.
 */
STree* appendSTree(STree* head, STree* t) {

    while ( head->next ) head = head->next;
    head->next = t;
    t->prev = head;
    t->next = 0;

    return streeHead(t);

}

/* returns a pointer to the head of a STree Linked List */
STree* streeHead(STree* t) {

    while ( t->prev ) t = t->prev;
    return t;

}

/* returns a pointer to the last element of a STree Linked List */
STree* streeTail(STree* t) {

    while ( t->next ) t = t->next;
    return t;

}

/* Inserts a STree t before the STree pointed to by after.
 * Returns the new head of the STree Linked List.
 * NOT YET TESTED.
 */
STree* insertBeforeST(STree* after, STree* t) {

    STree* left = after->prev;
    STree* right = after;
    if ( left ) left->next = t;
    t->prev = left;
    t->next = right;
    right->prev = t;

    return streeHead(after);

}

/* Inserts a STree t after the STree pointed to by before.
 * Returns the new head of the STree Linked List.
 * NOT YET TESTED.
 */
STree* insertAfterST(STree* before, STree* t) {

    STree* left = before;
    STree* right = before->next;
    if ( right ) right->prev = t;
    t->prev = left;
    t->next = right;
    left->next = t;

    return streeHead(before);

}


/* removes a STree from the Linked List it's in. Returns the new head of the LL. */
STree* removeSTree(STree* t) {

    STree* left = t->prev;
    STree* right = t->next;
    if ( left ) left->next = right;
    if ( right ) right->prev = left;

    freeSTree(t);

    if ( left ) return left;
    if ( right ) return right;
    return 0;

}

/* Splits a STree Linked List right before the node specified.
 * Returns a pointer to the head of the second list.
 */
STree* splitBeforeST(STree* t) {

    if ( !t->prev ) return t;
    t->prev->next = 0;
    t->prev = 0;
    return t;

}

/* Splits a STree Linked List right after the node specified.
 * Returns a pointer to the head of the second list.
 */
STree* splitAfterST(STree* t) {

    if ( !t->next ) return streeHead(t);
    STree* temp = t->next;
    t->next->prev = 0;
    t->next = 0;
    return temp;

}


/* Prints a Linked List of STrees from the start of the list. */
void printSTreeLL(STree* head) {

    while ( head->prev ) head = head->prev;
    while ( head ) {
        printSTree(head);
        head = head->next;
    }

}

/* Frees allocated memory for a STree object. */
void freeSTree(STree* t) {

    if ( t->value ) freeToken(t->value);
    free(t);

}

/* Frees allocated memory for a STree Linked List from the
 * start of the list.
 */
void freeSTreeLL(STree* head) {

    while ( head->prev ) head = head->prev;

    STree* next;
    while ( head ) {
        next = head->next;
        freeSTree(head);
        head = next;
    }

}


/* Takes a Token Linked List, and turns it into a STree Linked List. */
STree* tokenToSTree(Token* head) {

    if ( !head ) return newSTree(newToken());
    if ( !head->value ) return newSTree(newToken());
    head = tokenHead(head);
    STree* SThead = newSTree(newTokenV(head->value));
    SThead->prec = evaluatePrecedence(head->value[0]);
    head = head->next;
    STree* cur;
    while ( head ) {
        cur = newSTree(newTokenV(head->value));
        cur->prec = evaluatePrecedence(head->value[0]);
        appendSTree(SThead,cur);
        head = head->next;
    }

    return streeHead(SThead);

}

/* Takes a STree Linked List, and condenses out the highest-precedence
 * element. To condense, uses a Rule







