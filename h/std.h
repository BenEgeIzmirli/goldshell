#ifndef STD_H
#define STD_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include "types.h"
#include "utility.h"

unsigned int ERROR_CODE = 0;
int STD_BACKUP[2];
static char TEE[10];
static char HOME[10];
static Process* root;
static char buf[MAX_LINE_LENGTH] = {0};
static char promptText[128] = "(%d)gsh# ";

#endif
