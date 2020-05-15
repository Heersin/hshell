#include "autocomplete.h"
void auto_complete(char *buf, int len)
{
    // find
    // /usr/bin
    // /bin
    // ./
    // -name
    // buf word
    char *argv[7] = {"find", "/usr/bin", "/bin", "./", "-name", NULL, NULL};
    char *search_word;
    int tmp;

    search_word = (char *)malloc(sizeof(char) * (len + 2));
    for(tmp = 0; tmp < len; ++tmp)
        search_word[tmp] = buf[tmp];
    search_word[len] = '*';
    search_word[len + 1] = '\0';

    argv[5] = search_word;

    signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

    pid_t pid;
    int child_info;

    if((pid = fork()) == -1){
        perror("fork fail auto_complete");
        exit(-1);
    }

    // son
    if(pid == 0){
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
    }else
    {
        if ( wait(&child_info) == -1 ){
            free(search_word);
		    perror("wait");
        }
        free(search_word);
    }
}