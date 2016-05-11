#ifndef SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYZER_H

void printSTree(STree* head);
STree* newSTree(Token* t);
STree* appendSTree(STree* head, STree* t);
STree* streeHead(STree* t);
STree* streeTail(STree* t);
STree* insertBeforeST(STree* after, STree* t);
STree* insertAfterST(STree* before, STree* t);
STree* splitBeforeST(STree*);
STree* splitAfterST(STree*);
STree* removeSTree(STree* t);
void printSTreeLL(STree* head);
void freeSTree(STree* t);
void freeSTreeLL(STree* head);
STree* tokenToSTree(Token* head);
STree* condense(STree* head);

#endif
