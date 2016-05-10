#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

Token* newToken();
Token* newTokenV(char* value);
void printToken(Token*);
void printTokenLL(Token*);
Token* appendToken(Token* head, Token* t);
Token* insertToken(Token* after, Token* t);


#endif
