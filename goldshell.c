#include "interfaces/Tokenizer.c"

int main(int argc, char** argv) {

    char* buf = (char*) safe_malloc(MAX_LINE_LENGTH*sizeof(char));

    while ( NULL != fgets(buf,MAX_LINE_LENGTH,stdin) ) {

        Token* tokenchain = Tokenize(buf);
        printTokenLL(tokenchain);
        freeTokenLL(tokenchain);

    }
    free(buf);

}
