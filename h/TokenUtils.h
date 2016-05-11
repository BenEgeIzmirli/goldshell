#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

Token* newToken();
Token* newTokenV(char* value);
void printToken(Token*);
void printTokenLL(Token*);
void freeTokenLL(Token*);
void freeToken(Token*);
Token* copyToken(Token*);
Token* copyTokenLL(Token* head);
Token* appendToken(Token* head, Token* t);
Token* tokenHead(Token*);
Token* tokenTail(Token*);
Token* insertBefore(Token* after, Token* t);
Token* insertAfter(Token* before, Token* t);
Token* removeToken(Token* t);



#endif
