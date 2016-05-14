#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

void freeProcessNotCommand(Process*);

Token* newToken();
Token* newTokenV(char* value);
void printToken(Token*);
void printTokenPrepend(Token*,char*);
void printTokenLite(Token* t, char* p);
void printTokenValues(Token* head);
void printTokenValues_helper(Token* head, int prepend);
void printTokenLL(Token*);
void printTokenLL_helper(Token*,int);
void freeTokenLL(Token*);
void freeToken(Token*);
Token* copyToken(Token*);
Token* copyTokenLL(Token* head);
Token* copyTokenTree(Token* head);
Token* appendToken(Token* head, Token* t);
Token* tokenHead(Token*);
Token* tokenTail(Token*);
Token* insertBefore(Token* after, Token* t);
Token* insertAfter(Token* before, Token* t);
Token* splitBefore(Token*);
Token* appendToken(Token* dest, Token* src);
Token* splitAfter(Token*);
Token* removeToken(Token* t);
void freeTokenNotCommand(Token*);
Token* removeTokenNotCommand(Token*);
void freeProcess(Process*);



#endif
