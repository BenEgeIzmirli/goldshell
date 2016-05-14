#include "../h/std.h"
#include "../h/Executer.h"

Process* newProcess(pid_t pid) {

    Process* ret = safe_malloc(sizeof(Process));
    ret->children = 0;
    ret->parent = 0;
    ret->pid = pid;
    return ret;

}

Process* copyProcessTree(Process* p) {

    if ( !p ) return (Process*)0;
    Process* copy = newProcess(p->pid);
    copy->children = copyTokenTree(p->children);
    copy->command = copyTokenTree(p->command);
    Token* copyCur = copy->children;
    Token* pCur = p->children;
    while ( copyCur && pCur ) {
        copyCur->proc = copyProcessTree(pCur->proc);
        if ( copyCur->proc ) copyCur->proc->parent = copyCur;
        copyCur->parent = copy;
        copyCur = copyCur->next;
        pCur = pCur->next;
    }

    return copy;

}

/*
Process* my_fork(Token* command, Process* root) {

    pid_t pid = fork();
    if ( pid < 0 ) {
        ERROR_FAILED_FORK(pid);
        return (Process*)0;
    }
    if ( !pid ) return (Process*)0;
    Process* new = newProcess(pid);
    new->command = command;
    addChild(root,new);
    return new;

}
*/

void addChild(Process* root, Process* child) {

    Token* procToken = newToken();
    procToken->proc = child;
    procToken->parent = root;
    child->parent = procToken;
    if ( root->children ) appendToken(root->children,procToken);
    else root->children = procToken;

}

/* Takes a command, and sets the output file for this process to the
 * appropriate value, i.e. if the command ends in &, then redirects
 * output to /dev/null. Returns 0 if output location is still set to
 * the default stdout.
 */
/*
int negotiateOutput(Token* command) {

    if ( isBackground(command) ) {
        int fd[2] = {STDIN_FILENO,open("/dev/null", O_WRONLY)};
        return switchIO( fd );
    }
    return 0;

}
*/


Token* specialChar(Token* command) {

    if ( !command | !command->value ) return (Token*)0;
    for ( Token* cur=command ; cur ; cur = cur->next ) {
        if ( cur->symbol != (unsigned int)-1 ) return cur;
    }
    return (Token*)0;

}




void printProcess(Process* p) {

    printf("----- PROCESS %p -----\n",(void*)p);
    printf("| pid:      %d\n",(int)p->pid);
    printf("| command:  %p\n",(void*)p->command);
    printf("| parent:   %p\n",(void*)p->parent);
    printf("| children: %p\n",(void*)p->children);
    printf("----- END PROCESS -----\n");
    

}

void printProcessTree(Process* root) {

    printProcessTree_helper(root,0);

}


void printProcessTree_helper(Process* root, int prepend) {

    if ( !root ) return;
    char p[3*prepend+1];
    memset(p,' ',3*prepend);
    p[3*prepend] = 0;
    printf("%sPID=%d\n",p,(int)root->pid);
    if ( root->command ) printf("%sCOMMAND:%s\n",p,root->command->value);
    Token* ch = root->children;
    if ( !ch ) return;
    ch = tokenHead(ch);
    while ( ch ) {
        printProcessTree_helper(ch->proc,prepend+1);
        ch = ch->next;
    }

}

int reapProcess(Process* reap) {
    if ( !reap ) return 0;
    if ( !reap->parent ) return 0; // process is root process
    //freeTokenLL(reap->command);
    //reap->command = 0;
    Token* r = reap->parent;
    Process* p = r->parent;
    p->children = removeToken(r);
    return 1;

}

void reapZombies(Process* root) {

    if ( !root ) return;
    //reapProcess(root);
    Token* cur = root->children;
    Token* temp;
    while ( cur ) {
        temp = cur->next;
        if ( isRunning(cur->proc)==0 ) reapProcess(cur->proc);
        if ( ERROR_CODE ) break;
        cur = temp;
    }

}

void waitZombies(Process* root) {

    if ( !root ) return;
    //reapProcess(root);
    Token* cur = root->children;
    int oneRunning = 0;
    while ( cur ) {
        oneRunning += (int)cur->proc->pid;
        if ( isRunning(cur->proc)==0 ) {
            reapProcess(cur->proc);
            if ( ERROR_CODE ) break;
            cur = root->children;
            continue;
        }
        cur = cur->next;
        if ( !cur ) {
            if ( !oneRunning ) break;
            cur = root->children;
        }
    }

}

// return:
// -1 : not running, and hasn't been started, so don't reap
//  0 : ran, finished, pls reap
//  1 : still running
int isRunning(Process* p) {

    if ( !p->pid ) return -1;
    int result = 0;
    waitpid(p->pid, &result, WNOHANG );
    if ( WIFEXITED(result) ) {
        ERROR_CODE = WEXITSTATUS(result);
        return 0;
    }
    return 1;


}

void freeProcessNotCommand(Process* p) {

    if ( !p ) return;

    while ( p->children ) p->children = removeTokenNotCommand(p->children);

    free(p);
    return;

}

void freeProcess(Process* p) {

    if ( !p ) return;

    while ( p->children ) p->children = removeToken(p->children);

    if ( p->command ) freeTokenLL(p->command);

    free(p);
    return;

}


Arguments* newArguments() {

    Arguments* ret = safe_malloc(sizeof(Arguments));
    memset(ret, 0, sizeof(Arguments));
    return ret;
}

Arguments* newArgumentsV(Token* head) {

    if ( !head->leaf ) return (Arguments*)0;
    Arguments* ret = newArguments();
    for ( Token* cur=head ; cur ; cur = cur->next ) {
        if ( -1 == appendArg(ret,cur->value) ) ERROR_TOO_MANY_TOKENS();
    }

    return ret;

}

void populateArgs(Process* p) {

    Arguments* new = newArgumentsV(p->command);
    if ( !new ) return;
    memcpy(p->args,new->args,128);
    free(new);

}

int appendArg(Arguments* args, char* str) {

    if ( args->argct == 127 ) return -1;
    args->args[args->argct++] = str;
    return args->argct-1;

}


void printArguments(Arguments* args) {

    char* p = *args->args;
    while ( p++ ) printf("%s ",p);
    printf("\n");

}

void do_close(int fd) {

    if ( fd == STDIN_FILENO || fd == STDOUT_FILENO ) return;
    if ( -1 == close(fd) ) exit(errno);

}

void do_dup2(int new, int old) {

    if ( new == old ) return;
//printf("duping - copying %d into %d\n",new,old);
    if ( -1 == dup2(new,old) ) ERROR_DUP_ERROR();
//printf("duped - copying %d into %d\n",new,old);


}

void do_dup2_io(int fd[2]) {

    if ( fd[0] >= 0 ) do_dup2(fd[0],STDIN_FILENO);
    if ( fd[1] >= 0 ) do_dup2(fd[1],STDOUT_FILENO);
    //do_close(fd[0]);
    //do_close(fd[1]);

}


Process* do_fork(Process* root) {

    //if ( !root || !cmd ) return -1;
    pid_t pid;
    if ( -1 == (pid=fork()) ) {
        ERROR_FAILED_FORK(pid);
        return 0;
    }
    if ( pid == 0 ) {
        SET_SIGHANDLERS();
        return 0;
    }
    Process* p = newProcess(pid);
    p->command = root->command;
    addChild(root,p);
    return p;

}

void assignArgs(Process* p) {

    if ( !p ) return;

    char* func = p->command->value;

    if ( !strcmp( func, "exit" ) ) {
        EXIT_CMD_EXIT_SUCCESS();
        return;
    } else if ( !strcmp( func, "myinfo" ) ) {
        printf("\nMy PID: %d , my parent's PID: %d\n\n",(int)getpid(),(int)getppid());
        return;
    } else if ( !strcmp( func, "cd" ) ) {
        return;
    }
    populateArgs(p);
    return;

}

int check_cd(Token* t) {

    if ( !t ) return -1;
    for ( Token* cur = t ; cur ; cur = cur->next ) {
        if ( !strcmp( cur->value, "cd" ) ) {
            if ( !cur->next ) {
                chdir(getenv("HOME"));
                return 1;
            } else {
                if ( cur->next->value ) chdir(cur->next->value);
                else chdir(HOME);
                return 1;
            }
        }
        check_cd(cur->down);
        check_cd(cur->left);
        check_cd(cur->right);
    }
    return 0;

}

void choosePiping(Process* root, Process* left, Process* right, int p[2]) {

    #define rootin root->fd[0]
    #define rootout root->fd[1]
    #define leftin left->fd[0]
    #define leftout left->fd[1]
    #define rightin right->fd[0]
    #define rightout right->fd[1]
    #define pipein p[1]
    #define pipeout p[0]

    if ( root->command->leaf || !left | !right ) {
        assignArgs(root);
        return;
    }

    char s = root->command->value[0];
    if ( s == '|' ) {
        leftin   = rootin;
        leftout  = pipein;
        rightin  = pipeout;
        rightout = rootout;
        populateArgs(left);
        populateArgs(right);
    } else if ( s == '>' ) {
        leftin   = rootin;
        //leftout = open(left->command->value, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        //if ( leftout == -1 ) ERROR_OPENING_FILE(left->command->value);
        leftout  = pipein;
        //rightin  = rootin;  // value irrelevant
        //rightout = rootout; // value irrelevant
        rightin  = pipeout;
        rightout = open(right->command->value, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if ( rightout == -1 ) ERROR_OPENING_FILE(right->command->value);
        populateArgs(left);
        //populateArgs(right);
        right->args[0] = TEE;
    } else if ( s == '<' ) {
        leftin   = open(right->command->value, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if ( leftin == -1 ) ERROR_OPENING_FILE(left->command->value);
        leftout  = pipein;
        rightin  = pipeout;
        rightout = rootout;
        populateArgs(left);
        right->args[0] = TEE;
        //populateArgs(right);
    } else if ( s == '&' ) {
        leftin   = rootin;
        leftout  = pipein;
        rightin  = pipeout;
        rightout = open("/dev/null", O_WRONLY);
        if ( rightout == -1 ) ERROR_OPENING_FILE("/dev/null");
        populateArgs(left);
        right->args[0] = TEE;
        //populateArgs(right);
    } else {
        ERROR_SYNTAX_ERROR(root->command->value);
    }

}

void eval(Process* root) {

    if ( ERROR_CODE ) return;

    Token* cmd = root->command;
    if ( cmd->leaf ) {
        if ( do_fork(root) ) {
            waitZombies(root);
        } else {
            do_dup2_io(root->fd);
            if ( root->args[0] ) {
                execvp(root->args[0],root->args);
                ERROR_EXEC_ERROR(cmd);
            }
        }
        return;
    }

    int p[2];
    if ( -1 == pipe(p) ) ERROR_PIPE_ERROR();

    // make a process for the lefthand side of the pipe
    Process* left = newProcess(0);
    left->command = copyTokenTree(cmd->left);
    left->left = 1;
    addChild(root,left);

    // make a process for the righthand side of the pipe
    Process* right = newProcess(0);
    right->command = copyTokenTree(cmd->right);
    right->right = 1;
    addChild(root,right);

    // assign piping between them according to the type of pipe
    choosePiping(root,left,right,p);

    eval(left);
    do_close(p[1]); // close the pipe write end bc if you don't nothing works
    eval(right);

}


/*
void printfd(int fd[2]) {

    printf("in:%d\nout:%d\n-----\n",fd[0],fd[1]);

}

int switchIO(int pipe[2]) {

    if ( pipe[0] != STDIN_FILENO ) {
printf("changing stdin from %d to %d\n",STDIN_FILENO,pipe[0]);
        dup2(pipe[0],STDIN_FILENO);
printf("STDIN_FILENO is now %d\n",STDIN_FILENO);
        if ( dup2(pipe[0], STDIN_FILENO) ||
             close(pipe[0]) ) ERROR_PIPE_ERROR();
        if ( ERROR_CODE ) return 0;
    }
    if ( pipe[1] != STDOUT_FILENO ) {
printf("changing stdout from %d to %d\n",STDOUT_FILENO,pipe[1]);
        if ( dup2(pipe[1], STDOUT_FILENO) ||
             close(pipe[1]) ) ERROR_PIPE_ERROR();
        if ( ERROR_CODE ) return 0;
    }
    return 1;

}

int resetIO() {

    int temp[2] = {STDIN_FILENO,STDOUT_FILENO};
    if ( STDIN_FILENO != STD_BACKUP[0] ) {
        if ( dup2(STD_BACKUP[0],0) ||
             close(temp[0]) ) ERROR_PIPE_ERROR();
    }
    if ( STDOUT_FILENO != STD_BACKUP[1] ) {
        if ( dup2(STD_BACKUP[1],1) ||
             close(temp[1]) ) ERROR_PIPE_ERROR();
    }

}


void my_exec(Token* command, Process* root, int* pipe) {
printf("\n\n\n----------------\nHere in my_exec with %s.\n",command->value);

printf("execing %s between %d and %d\n",command->value, pipe[0], pipe[1]);

dprintf(STD_BACKUP[1],"STDIN: %d valid: %d ... STDOUT: %d valid: %d\n",STDIN_FILENO,fcntl(STDIN_FILENO, F_GETFD) != -1,STDOUT_FILENO,fcntl(STDOUT_FILENO, F_GETFD) != -1);
    switchIO(pipe);
    Arguments* args = newArgumentsV(command);
dprintf(STD_BACKUP[1],"command:%s STDIN_FILENO: %d STDOUT_FILENO: %d\n",command->value,STDIN_FILENO,STDOUT_FILENO);
dprintf(STD_BACKUP[1],"fd %d valid: %d ... fd %d valid: %d\n",pipe[0],fcntl(pipe[0], F_GETFD) != -1,pipe[1],fcntl(pipe[1], F_GETFD) != -1);
dprintf(STD_BACKUP[1],"STDIN: %d valid: %d ... STDOUT: %d valid: %d\n",STDIN_FILENO,fcntl(STDIN_FILENO, F_GETFD) != -1,STDOUT_FILENO,fcntl(STDOUT_FILENO, F_GETFD) != -1);
printf("\n-------------------\n\n\n");
    execvp(args->args[0],args->args);
    ERROR_EXEC_ERROR(command);
    exit(ERROR_CODE);

}


void evalCommand_helper(Token* cmd, Process* root, int* fd) {

    if ( ERROR_CODE ) return;
    if ( cmd->leaf ) {
printf("\n\n\n----------------\nHere with %s, about to do a my_exec.\n",cmd->value);
dprintf(STD_BACKUP[1],"fd %d valid: %d ... fd %d valid: %d\n",fd[0],fcntl(fd[0], F_GETFD) != -1,fd[1],fcntl(fd[1], F_GETFD) != -1);
printf("\n------------------\n\n\n");
        Process* p;
        if ( !(p = my_fork(cmd,root)) ) my_exec(cmd,root,fd);
        for ( int i=0 ; i<1000000 ; i++ );
    } else {
        // figure out why it's not a leaf, evaluate those first
        Token* sym = specialChar(cmd);
        if ( sym && !isUnary(sym->value[0]) ) {
//printf("unary\n");
            // do stuff with down
        } else { // do stuff with left and right
//printf("not unary\n");
            if ( sym->next || sym->prev ) {
                printf("this shouldn't happen.\n");
                exit(15);
            }
            if ( !sym->left || !sym->right ) {
                printf("this shouldn't happen either.\n");
                exit(16);
            }
            int bridge[2];
            if ( -1 == pipe(bridge) ) ERROR_PIPE_ERROR();
printf("made bridge between %d and %d\n",bridge[0],bridge[1]);
printf("STD_BACKUP[0]:%d, STD_BACKUP[1]:%d\n",STD_BACKUP[0],STD_BACKUP[1]);
            int leftfd[2] = {fd[0],bridge[0]};
printf("leftfd[0]:%d leftfd[1]:%d\n",leftfd[0],leftfd[1]);
            int rightfd[2] = {bridge[1],fd[1]};
printf("rightfd[0]:%d rightfd[1]:%d\n",rightfd[0],rightfd[1]);
printf("evaluating left:\n");
            evalCommand_helper(cmd->left,root,leftfd);
printf("evaluating right:\n");
            evalCommand_helper(cmd->right,root,rightfd);
        }
    }
}
*/

/*
void evalCommand_helper(Token* cmd, Process* root, int fd[2]) {

    Process* p;
    if ( (p=my_fork(cmd,root)) ) {
        waitZombies(root);
    } else {
        if ( cmd->leaf ) {
            Arguments* args = newArgumentsV(cmd);
printArguments(args);
//printf("myfd:\n");
//printfd(fd);
            my_exec(cmd,root,fd);
        }
        int bridge[2];
        if ( pipe(bridge) ) {
            ERROR_CREATING_PIPE();
            return;
        }
        int leftfd[2] = {fd[0],bridge[0]};
//printf("left:\n");
//printfd(leftfd);
        int rightfd[2] = {bridge[1],fd[1]};
//printf("right:\n");
//printfd(rightfd);
        evalCommand_helper(cmd->right,root,rightfd);
//printTokenLL(root->children);
        evalCommand_helper(cmd->left,root,leftfd);
        resetInput();
        resetOutput();
    }

}
*/


/*
void evalCommand_helper(Token* cmd, Process* root, int fd[2]) {
dprintf(fd[1],"here I am\n");
//printf("in evalcomandhelp\n");
//printTokenLL(cmd);
if ( ERROR_CODE ) return;
    if ( cmd->leaf ) {
        // exec cmd using given outfile descriptor,
        // pipe in given infile descriptor
        dprintf(fd[1],"ayy lmao\n");
        char c;
        while ( EOF != (c=fgetc((FILE*)fd[0])) ) dprintf(fd[1],"%c",c);
    } else {
        // figure out why it's not a leaf, evaluate those first
        Token* sym = specialChar(cmd);
//printf("#####\n");
//printToken(sym);
//printf("#####\n");
        if ( sym && !isUnary(sym->value[0]) ) {
//printf("unary\n");
            // do stuff with down
        } else { // do stuff with left and right
            // make pair of file descriptors for left
            // make pair of file descriptors for right
            // for now, just populate args appropriately. eventually:
                // evalcmdhlpr left, piping output into right fd
                // evalcmdhlpr right, piping output into outf
            // return outf;
printf("not unary\n");
            if ( sym->next || sym->prev ) {
                printf("this shouldn't happen.\n");
                exit(15);
            }
            if ( !sym->left || !sym->right ) {
                printf("this shouldn't happen either.\n");
                exit(16);
            }
            //int left[2];
            //int right[2];
            //if ( pipe(left) | pipe(right) ) ERROR_CREATING_PIPE();
            int bridge[2];
            if ( pipe(bridge) ) ERROR_CREATING_PIPE();
dprintf(fd[1],"test %d\n",ERROR_CODE);
            Process* leftP;
            if ( !(leftP = my_fork(sym->left,root)) ) {
                printf("~~~~Evaluating left:\n");
                switchInput(fd[0]);
                switchOutput(bridge[0]);
                evalCommand_helper(sym->left,root,fd);
                resetOutput();
                printf("exiting left.\n");
                exit(ERROR_CODE);
            }
            Process* rightP;
            if ( !(rightP = my_fork(sym->right,root)) ) {
                printf("~~~~Evaluating right:\n");
                switchInput(bridge[1]);
                switchOutput(fd[1]);
                evalCommand_helper(sym->right,root,fd);
                resetOutput();
                printf("exiting right\n");
                exit(ERROR_CODE);
            }
dprintf(fd[1],"should go to stdout\n");
        }
    }

}
*/

/*
void execForMe(Token* cmd, int fd[2]) {

    int inBackup = dup(STDIN_FILENO);
    int outBackup = dup(STDOUT_FILENO);
    switchInput(fd[0]);
    switchInput(fd[1]);
    evalCommand_helper(sym->left,

}
*/













