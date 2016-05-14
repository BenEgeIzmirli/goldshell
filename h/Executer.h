#ifndef EXECUTER_H
#define EXECUTER_H

Token* removeTokenNotCommand(Token*);

Process* newProcess(int pid);
Process* copyProcess(Process* p);
Process* my_fork(Token* command, Process* root);
void addChild(Process* root, Process* child);
int negotiateOutput(Token* command);
void printProcessTree(Process* root);
Token* specialChar(Token* command);
void printProcessTree_helper(Process* root,int prepend);
int reapProcess(Process* reap);
void reapZombies(Process* root);
void waitZombies(Process* root);
int isRunning(Process*);
void freeProcess(Process*);
void freeProcessNotCommand(Process*);
void my_exec(Token* command, Process* root, int* pipe);
void evalCommand_helper(Token* cmd, Process* root, int* fd);
Arguments* newArguments();
Arguments* newArgumentsV(Token* head);
int appendArg(Arguments* args, char* str);
int switchIO(int pipe[2]);
int resetIO();




#endif
