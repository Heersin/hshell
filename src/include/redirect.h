#ifndef _H_REDIRECT
#define _H_REDIRECT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "execute.h"

int std_out_fd;
int std_in_fd;

// parse and call the following
// only support :
// 1. cmd [args] < file
// 2. cmd [args] >> file
// 3. cmd [args] > file
// the space is required, and only between 2 objects
int start_redirect(char **arglist);

void init_redirect();

// cmd > file.txt
void redirect_write_to(char *filename);

// cmd >> file.txt
void redirect_append_to(char *filename);

// cmd < file.txt
void redirect_input_from(char *filename);

void end_redirect();
#endif