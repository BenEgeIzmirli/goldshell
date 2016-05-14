#ifndef TABLES_H
#define TABLES_H

// Ordered from highest to lowest precedence.
// To specify a non-unary operator (one that needs a complement,
// like graves or parentheses), type the symbol twice successively.
const char* const _SYMBOLS[] = {
//    "\\",                 // when these are not commented out, escape slashes, grave symbols,
//    "``",                 // and parentheses will also be independently and appropriately
//    "(()){{}}[[]]",       // factored into the parse tree.
    "|><&"
};

int evaluatePrecedence(char);
int containsSymbol(char* str);
int isUnary(char);
int symbolComplements(char sym1, char sym2);
int isDirectional(char sym);
int directionalComplement(char sym1, char sym2);
int isBackground(Token* command);
Token* condenseP(Token* sym);
int stealUpToken(Token* self);
void unaryPlaceholder();
Token* condenseQ(Token* startQ, Token* endQ);

#endif
