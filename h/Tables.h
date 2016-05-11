#ifndef TABLES_H
#define TABLES_H
// Ordered from highest to lowest precedence.
const char* const _SYMBOLS[] = {
    "''",
    "\\",
    "``",
    "\"\"",
    "|><&"
};

// Specify whether each tier of _SYMBOLS consists of unary symbols or not.
const int const _UNARY[] = { 0,1,0,0,1 };

int evaluatePrecedence(char);
int containsSymbol(char* str);
int isUnary(char);

#endif
