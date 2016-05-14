#include "../h/std.h"
#include "../c/Executer.c"



void evalCommand(Process* root) {

    if ( !root ) return;

    int temp[2] = {0,0};
    choosePiping(root,0,0,temp);
    if ( ERROR_CODE ) return;

    root->fd[0] = -1;
    root->fd[1] = -1;
    eval(root);
    int status = 0;
    wait(&status);
    print_prompt((int)getppid());

}













