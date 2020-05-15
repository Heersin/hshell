#ifndef _H_PIPE
#define _H_PIPE

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_PIPE_CMD 8
int is_pipe_cmd(char *cmd);
int start_pipe(char **arglist);
char **next_pipe_cmd();
void _pipe_execute(int *father_pipe, int fork_level);
void _pipe_modify(char **arglist, int *left_pipe, int *right_pipe);
void pipe_execute();

#endif