#ifndef SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYZER_H

Token* setPrecedences(Token* head);
Token* setTreePrecedences(Token* head);
Token* findHighestPrecedence(Token* head);
Token* findComplement(Token* start);
Token* refactorTier(Token* head);

#endif
