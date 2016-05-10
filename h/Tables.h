#ifndef TABLES_H
#define TABLES_H
/*
const char* _SYM0 = "'";
const char* _SYM1 = "\\";
const char* _SYM2 = "`";
const char* _SYM3 = "\"";
const char* _SYM4 = "()";
const char* _SYM5 = "|><&";

const char* _SYMBOLS[6] = {
    _SYM0, _SYM1, _SYM2, _SYM3, _SYM4, _SYM5
};
*/
const char* const _SYMBOLS[] = {
    "'",
    "\\",
    "`",
    "\"",
    "()",
    "|><&"
};
int evaluatePrecedence(char);

#endif
