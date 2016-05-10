#include "../h/std.h"
#include "../c/Tokenizer.c"

/* Turns a line of text into a Linked List of
 * Token objects. Returns the pointer to the head
 * of the Linked List. Uses a Finite State Machine
 * implementation to change all space characters
 * in str into null bytes, and creates a Token
 * for each word.
 */
Token* Tokenize(char* str) {

    Token* head = strToToken(str);

    if ( !head->value ) return head;

    //symbolSplit(head);

    return head;

}









