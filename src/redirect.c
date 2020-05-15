#include "redirect.h"
int is_redirect_cmd(char *cmd)
{
    if((strchr(cmd, '>') == NULL) && (strchr(cmd, '<') == NULL))
        return 0;
    else
        return 1;
}

int start_redirect(char **arglist)
{
    char **left;
    char *mode;
    char *right;

    // count
    int argc = 0;
    if(arglist == NULL || arglist[0] == NULL){
        perror("error parse in redirect");
        return -1;
    }

    while(arglist[++argc] != NULL);

    if(argc < 3){
        perror("too less to redirect");
        return -1;
    }

    left = arglist;
    mode = arglist[argc - 2];
    right = arglist[argc - 1];

    if(strstr("> >> <", mode) == NULL){
        perror("Not support redirect");
        return -1;
    }

    init_redirect();

    if(strcmp(">", mode) == 0){
        redirect_write_to(right);
        left[argc - 2] = NULL;
        execute(left);
    }
    else if(strcmp(">>", mode) == 0){
        redirect_append_to(right);
        left[argc - 2] = NULL;
        execute(left);
    }
    else{
        redirect_input_from(right);
        left[argc - 2] = NULL;
        execute(left);
    }

    end_redirect();
    return 0;
}

void init_redirect()
{
    // save the origin stdout
    std_out_fd = dup(STDOUT_FILENO);
    // save the input
    std_in_fd = dup(STDIN_FILENO);
}


// cmd > file.txt
void redirect_write_to(char *filename)
{   
    // the last arg is about security issue
    int fd = open(filename, O_CREAT|O_TRUNC|O_WRONLY);
    // stdout now is fd
    dup2(fd, STDOUT_FILENO);
    // close fd
    close(fd);
}


// cmd >> file.txt
void redirect_append_to(char *filename)
{   
    // the last arg is about security issue
    int fd = open(filename, O_CREAT|O_APPEND|O_WRONLY);
    // stdout now is fd
    dup2(fd, STDOUT_FILENO);
    // close fd
    close(fd);
}


// cmd < file.txt
void redirect_input_from(char *filename)
{
    int fd = open(filename, O_RDONLY);
    dup2(fd, STDIN_FILENO);
    close(fd);
}


void end_redirect()
{
    dup2(std_out_fd, STDOUT_FILENO);
    dup2(std_in_fd, STDIN_FILENO);
}
