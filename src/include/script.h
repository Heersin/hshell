#ifndef _SCRIPT_H
#define _SCRIPT_H

#include	<stdio.h>
int syntax_err(char *msg);
int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
#endif