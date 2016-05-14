#ifndef GOLDSHELL_TYPES
#define GOLDSHELL_TYPES

typedef struct Token {

    char* value;
    unsigned int prec;
    unsigned int symbol;
    unsigned int leaf;
    struct Process* proc;
    struct Process* parent;

    struct Token* next;
    struct Token* prev;

    struct Token* up;
    struct Token* down;
    struct Token* left;
    struct Token* right;

} Token;

typedef struct Process {

    pid_t pid;
    struct Token* command;
    int fd[2];
    char* args[128];
    int left,right;
    int chdirpipe;

    struct Token* parent;
    struct Token* children;

} Process;

typedef struct Arguments {

    int argct;
    char* args[128];

} Arguments;


#endif
