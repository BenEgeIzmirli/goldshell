#ifndef GOLDSHELL_TYPES
#define GOLDSHELL_TYPES

typedef struct Token {
                     //for type:
    char* value;     //-1=verb
    char type;       // 0=other
    char precendence;// 1=symbol

    Token* next;
    Token* prev;

} Token;

typedef struct SymbolTable {

    Token* head;
    int tableLength;

} SymbolTable;

#endif
