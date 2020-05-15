#ifndef _PROC_H
#define _PROC_H
#include	<stdio.h>
#include "script.h"
#include "builtin.h"
#include "execute.h"
#include "pipe.h"
#include "redirect.h"

int process(char **args, char *cmdline);
#endif