#ifndef GOLDSHELL_TYPES
#define GOLDSHELL_TYPES

typedef struct Token {

    char* value;
    unsigned int verb:1;
    unsigned int symbol:1;
    unsigned int argument:1;
    unsigned int precedence:5;

    struct Token* next;
    struct Token* prev;

} Token;

#endif
