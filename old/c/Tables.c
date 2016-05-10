#include "../h/tables.h"
#include "../h/std.h"

SymbolTable* newSymbolTable() {

    void* ret = malloc(sizeof(SymbolTable));
    if ( ret == NULL ) {
        printf("memory allocation failed. exiting.\n");
        exit(EXIT_FAILURE);
    }
    ret = (SymbolTable*)ret;
    ret->symbols[0] = 0;
    return ret;

}

void insert_symbol(SymbolTable* st, char* symbol) {

    void* ret = malloc(strlen(symbol)*sizeof(char));
    if ( ret == NULL ) {
        printf("memory allocation failed. exiting.\n");
        exit(EXIT_FAILURE);
    }
    ret = (char*)ret;
    strcpy(ret,symbol);

    int i;
    for ( i=0 ; st->symbols[i] ; i++ );
    st->symbols[i] = ret;

}
