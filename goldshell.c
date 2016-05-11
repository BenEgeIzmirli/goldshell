#include "interfaces/Tokenizer.c"
#include "interfaces/SAnalyzer.c"

int main() {

    char* buf = (char*) safe_malloc(MAX_LINE_LENGTH*sizeof(char));

    while ( NULL != fgets(buf,MAX_LINE_LENGTH,stdin) ) {

        Token* tokenchain = Tokenize(buf);
        STree* command = constructSTree(tokenchain);
        //printTokenLL(tokenchain);
        freeTokenLL(tokenchain);

    }
    free(buf);

}
