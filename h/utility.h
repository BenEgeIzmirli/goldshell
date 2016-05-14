#ifndef UTILITY_H
#define UTILITY_H

#define MAX_LINE_LENGTH 1024

#define SET_SIGHANDLERS()                            \
    if ( signal(SIGINT,sig_handler) == SIG_ERR) {    \
        printf("WARNING: can't capture SIGINT.\n");  \
    }                                                \
    if ( signal(SIGTSTP,sig_handler) == SIG_ERR) {   \
        printf("WARNING: can't capture SIGTSTP.\n"); \
    }

void reapZombies(Process* root);

extern unsigned int ERROR_CODE;
extern int STD_BACKUP[2];
static char TEE[10] = "tee";
static char HOME[10] = "~";
static Process* root;
static char buf[MAX_LINE_LENGTH];
static char promptText[128];

static void print_prompt(int pid) {
    dprintf(STD_BACKUP[1],promptText,pid);
}

// used to exit using an error code
static void EXIT_ERROR_CODE(int EC) {
    if ( EC == -1 ) {
        printf("Goodbye!\n");
    } else {
        printf("Exiting with error code %d.\n",EC);
    }
    kill(getppid(),SIGKILL);
}

// ERROR_CODE -1 (SUCCESS)
static void EXIT_CMD_EXIT_SUCCESS() {
    ERROR_CODE = -1;
}

// ERROR_CODE 1
static void ERROR_NO_MATCHING_SYMBOL(Token* unmatched) {
    printf("Invalid syntax - no matching symbol for %s\n",unmatched->value);
    ERROR_CODE = 1;
}

// ERROR_CODE 2
static void ERROR_MISSING_ARGUMENT(Token* missingArg) {
    printf("Invalid syntax - missing argument for %s\n",missingArg->value);
    ERROR_CODE = 2;
}

// ERROR_CODE 3
static void ERROR_FAILED_FORK(pid_t pid) {
    printf("Failed fork with pid %d\n",(int)pid);
    ERROR_CODE = 3;
}

// ERROR_CODE 4
static void ERROR_TOO_MANY_TOKENS() {
    printf("Too many tokens on command line.\n");
    ERROR_CODE = 4;
}

// ERROR_CODE 5
static void ERROR_PIPE_ERROR() {
    printf("Error handling pipe\n");
    ERROR_CODE = 5;
}

// ERROR_CODE 6
static void ERROR_EXEC_ERROR(Token* cmd) {
    printf("Error at command starting with token %s (errno=%d)\n",cmd->value,errno);
    ERROR_CODE = 6;
}

// ERROR_CODE 7
static void ERROR_OPENING_FILE(char* filename) {
    printf("Error opening file %s\n",filename);
    ERROR_CODE = 7;
}

// ERROR_CODE 8
static void ERROR_SYNTAX_ERROR(char* problem) {
    printf("Syntax error at token %s\n",problem);
    ERROR_CODE = 8;
}

// ERROR_CODE 9
static void ERROR_DUP_ERROR() {
    printf("Error duping file descriptor.\n");
    ERROR_CODE = 9;
}

static void sigtstp_handler(Process* p) {

    if ( !p->children ) { // if we're at the rightmost node, background process
        p->fd[1] = open("/dev/null", O_WRONLY);
        if ( p->fd[1] == -1 ) ERROR_OPENING_FILE("/dev/null");
    } else {
        Token* cur = p->children;
        while ( cur ) {
            if ( cur->proc->right ) kill(cur->proc->pid,SIGTSTP);
        }
    }
}


// Sends the given signal recursively to all forked child processes.
static void sig_handler_helper(Process* p, int signum) {

    if ( p->children ) {
        Token* cur = p->children;
        while ( cur ) sig_handler_helper(cur->proc,signum);
    }
    kill(p->pid,signum);
    return;

}


static void sig_handler(int signum) {

    if ( signum == SIGINT ) {
        for ( Token* cur = root->children ; cur ; cur = cur->next ) {
            sig_handler_helper(root,SIGKILL);
        }
        reapZombies(root);
    }
    if ( signum == SIGTSTP ) {
        for ( Token* cur = root->children ; cur ; cur = cur->next ) {
            sigtstp_handler(cur->proc);
        }
    }

}




static void* safe_malloc(size_t size) {

    void* ret = malloc(size);
    if ( ret == NULL ) {
        printf("memory allocation failed. exiting.\n");
        exit(EXIT_FAILURE);
    }
    memset(ret,0,size);
    return ret;

}

static char* myStrndup(char* s) {

    int length = (strlen(s) > MAX_LINE_LENGTH) ? MAX_LINE_LENGTH : strlen(s)+1 ;
    char* ret = (char*) safe_malloc(length*sizeof(char));
    return strncpy(ret,s,length-1);

}

#endif

