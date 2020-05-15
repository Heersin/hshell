#ifndef _BUILT_H
#define _BUILT_H

#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	"varlib.h"


int assign(char *);
int okname(char *);
int builtin_command(char **args, int *resultp);
#endif