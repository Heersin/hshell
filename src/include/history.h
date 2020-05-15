#ifndef _HISTORY_H
#define _HISTORY_H

#include <stdio.h>
#include <stdlib.h>

/* Max record history */
#define MAX_HISTORY 1000

static char **history[MAX_HISTORY];

char *new_string(char *source, int len);
void init_history(char *filename);
void free_history();
void inqueue(char *string);
void outqueue();
char *locate_history(int index);
void init_cursor(int cursor);
char *previous_history();
char *next_history();
void write2history(char *filename);
void addhistory(char *string, int len);
#endif