#include "interfaces/Tokenizer.c"
#include "interfaces/SAnalyzer.c"
#include "interfaces/Executer.c"

static Process* root;

int main() { 

    printf("\n~~~~~~~~~~~~~~~~~~~~~\nWelcome to GoldShell!\n~~~~~~~~~~~~~~~~~~~~~\n\n");

    SET_SIGHANDLERS();

    STD_BACKUP[0] = dup(STDIN_FILENO);
    STD_BACKUP[1] = dup(STDOUT_FILENO);

    root = newProcess(getpid());
    int status;
    print_prompt((int)getpid());
    while ( NULL != fgets(buf,MAX_LINE_LENGTH,stdin) ) {
        waitpid(-1,&status,WNOHANG);
        if ( WIFEXITED(status) && WEXITSTATUS(status) ) break;
        if ( strlen(buf) < 2 ) {
            print_prompt((int)getpid());
            continue;
        }

        Token* tokenchain = Tokenize(buf);
        if ( ERROR_CODE ) {
            freeTokenLL(tokenchain);
            freeProcessNotCommand(root);
            EXIT_ERROR_CODE(ERROR_CODE);
        }

        Token* command = constructSTree(tokenchain);
        if ( ERROR_CODE ) {
            freeTokenLL(tokenchain);
            freeProcessNotCommand(root);
            EXIT_ERROR_CODE(ERROR_CODE);
        }
        if ( 1 == check_cd(command) ) {
            print_prompt((int)getpid());
            continue;
        }
        root->command = copyTokenTree(command);
        freeTokenLL(command);
        if ( !do_fork(root) ) {
            evalCommand(root);
            if ( ERROR_CODE ) {
                freeTokenLL(tokenchain);
                freeProcessNotCommand(root);
                EXIT_ERROR_CODE(ERROR_CODE);
            }
            return 0;
        }

        if ( ERROR_CODE ) {
            freeTokenLL(tokenchain);
            freeProcessNotCommand(root);
            EXIT_ERROR_CODE(ERROR_CODE);
        } 
        waitZombies(root);


    }
    freeProcessNotCommand(root);
    exit(EXIT_SUCCESS);

}







