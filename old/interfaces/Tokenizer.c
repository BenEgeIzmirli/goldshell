#include "../h/std.h"
#include "../c/Tokenizer.c"

Token* Tokenize(char*);

Token* Tokenize(char* str) {

    int i;
    char c;
    while ( (c=str[i++]) ) {
        printf("%c",c);
    }

}
