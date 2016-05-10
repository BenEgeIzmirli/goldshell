#ifndef UTILITY_H
#define UTILITY_H

#define MAX_LINE_LENGTH 1024

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

