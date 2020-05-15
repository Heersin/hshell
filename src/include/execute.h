#ifndef _EXE_H
#define _EXE_H
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	"varlib.h"
int execute(char *argv[]);

#define MAX_ARGV 16
#define MAX_ARGLEN 64
#endif