#ifndef GOLDSHELL_TYPES
#define GOLDSHELL_TYPES

typedef struct Token {

    char* value;

    struct Token* next;
    struct Token* prev;

} Token;

typedef struct STree {

    struct Token* value;
    int prec;

    struct STree* next;
    struct STree* prev;

    struct STree* down;
    struct STree* left;
    struct STree* right;

} STree;

#endif
