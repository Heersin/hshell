#ifndef _HSH_H
#define _HSH_H

#define YES 1
#define NO 0

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include "varlib.h"
#include "utils.h"
#include "process.h"
#include "history.h"

void setup();
void closedown();
#endif