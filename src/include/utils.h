#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "history.h"
#include "execute.h"
#include "autocomplete.h"
#include "varlib.h"
#define BUFSIZ 64
char *next_cmd(char *prompt, FILE *fp);
char **splitline(char *line);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);
void fatalerror(char *, char *, int);
char *newstr(char *s, int l);



#define CTRL_D 0x4
#define CTRL_L 0xC
#define TAB 0x9
#define ESC 0x1B

// depecrated
#define UP_KEY 38
#define LEFT_KEY 37
#define RIGHT_KEY 39
#define DOWN_KEY 40

#endif
