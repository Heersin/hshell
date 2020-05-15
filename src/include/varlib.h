/*
 * header for varlib.c package
 */
#ifndef _VARLIB_H
#define _VARLIB_H
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
int	VLenviron2table(char **);
int	VLexport(char *);
char	*VLlookup(char *);
void	VLlist();
int	VLstore( char *, char * );
char	**VLtable2environ();
int	VLenviron2table(char **);
void freetab();
#endif