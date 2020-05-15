#include "hsh.h"

/* Unix-like shell */

#define SHELL_PROMPT "$>> "

/* UNIX environ global , do not free */
extern char **environ;

int main()
{
    char *cmdline;
    char *prompt;
    char **arglist;
    int result;

    prompt = SHELL_PROMPT;
    // init some shell related attribute
    // TODO: read the config file and init shell
    setup();

    while((cmdline = next_cmd(prompt, stdin)) != NULL){
        if((arglist = splitline(cmdline)) != NULL){
            environ = VLtable2environ();
            result = process(arglist, cmdline);
            freelist(arglist);
            free(environ);
        }
        free(cmdline);
    }
    closedown();
    return 0;
}


void setup()
{
    VLenviron2table(environ);
    /* termios for non-buffer getc */
    /* Also Disable Echo */
    struct termios shell_tio;
    tcgetattr(STDIN_FILENO, &shell_tio);        // get tio of stdin
    shell_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &shell_tio);   // set tio for stdin

    /* Ignore some signals for shell */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    init_history(NULL);
}


void closedown()
{
    struct termios shell_tio;
    tcgetattr(STDIN_FILENO, &shell_tio);        // get tio of stdin
    shell_tio.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &shell_tio);   // restore tio for stdin
    freetab();
    free_history();
}