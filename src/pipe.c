#include "pipe.h"

int indexs[MAX_PIPE_CMD];
int cmd_index;
int indexs_len;
char **cmd_set;
int saved_fd[2];

// parse, and store for next_pipe_cmd to fetch
int is_pipe_cmd(char *cmd)
{
    if(strchr(cmd, '|') == NULL)
        return 0;
    else
        return 1;
}

int start_pipe(char **arglist)
{
    int cmd_cnt;
    int tmp;
    int argc;
    char *cur_arg;

    cmd_cnt = 0;
    tmp = 0;

    if(arglist == NULL || arglist[0] == NULL){
        perror("arglist in pipe error");
        return -1;
    }

    // handle the | in the head first
    if(strcmp(arglist[0], "|") == 0){
        perror("pipe nothing");
        return -1;
    }

    while(arglist[tmp] != NULL)
        tmp += 1;

    argc = tmp;

    // handle the | in the end
    if(strcmp(arglist[tmp - 1], "|") == 0){
        perror("pipe remain | in the end");
        return -1;
    }

    // the first cmd from 0;
    indexs[0] = 0;
    cmd_cnt = 1;

    // cut cmds and record each cmd index of their start address
    for(tmp = 0; tmp < argc; ++tmp){
        if(cmd_cnt >= MAX_PIPE_CMD){
            perror("Reach the max cmd to pipe");
            return -1;
        }

        if(strcmp(arglist[tmp], "|") == 0){
            indexs[cmd_cnt] = tmp + 1;      // record the next cmd start
            cmd_cnt += 1;                   
            arglist[tmp] = NULL;            // cut
        }
    }

    // init global var for pipe
    cmd_index = 0;
    indexs_len = cmd_cnt;
    cmd_set = arglist;
    
    // execute
    pipe_execute();
    return 0;
}

char **next_pipe_cmd()
{
    char **result;
    if(cmd_index < indexs_len){
        result = cmd_set+indexs[cmd_index];
        cmd_index += 1;
        return result;
    }
    return NULL;
}

void pipe_execute()
{
    saved_fd[0] = dup(STDIN_FILENO);
    saved_fd[1] = dup(STDOUT_FILENO);
    pid_t pid;
    int fork_level = indexs_len - 1;
    int child_info = -1;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    if((pid = fork()) < 0)
    {
        perror("fork fail in pipe_execute()");
        exit(-1);
    }

    // children
    if(pid == 0)
        _pipe_execute(NULL, fork_level);
    else
        if(wait(&child_info) == -1)
            perror("Wait");
}

void _pipe_modify(char **arglist, int *left_pipe, int *right_pipe)
{
    if(left_pipe != NULL){
        // 把左边管道(父->子)的输入作为stdin
        dup2(left_pipe[0], STDIN_FILENO);
        // 已有修改后的stdin作为文件计数，可删去
        close(left_pipe[0]);
        close(left_pipe[1]);
    }

    if(right_pipe != NULL){
        dup2(right_pipe[1], STDOUT_FILENO);
        close(right_pipe[0]);
        close(right_pipe[1]);
    }

    // execute args
    execvp(arglist[0], arglist);
	perror("cannot execute command");
	return 0;
}


void _pipe_execute(int *father_pipe, int fork_level)
{
    char **cur_cmd;
    int fd[2];
    int *right_pipe;
    pid_t pid;

    // init
    right_pipe = NULL;

    // 获取cmd
    cur_cmd = next_pipe_cmd();
    if(cur_cmd == NULL || cur_cmd[0] == NULL)
    {
        if(fork_level == 0)
            return;
        printf("No command to pipe\n");
        return;
    }

    if(fork_level > 0){
        pipe(fd);
        right_pipe = &fd;
        fork_level -= 1;

        if( (pid = fork()) < 0)
        {
            perror("fork fail");
            exit(-1);
        }
    }
    
    // father
    if(pid > 0)
        _pipe_modify(cur_cmd, father_pipe, right_pipe);
    else if( (pid == 0) && (fork_level >= 0))
    {
        // still some commands
        if(father_pipe != NULL)
        {
            // close father pipe's fd, which is used and won't be used again
            close(father_pipe[0]);
            close(father_pipe[1]);
        }

        _pipe_execute(right_pipe, fork_level);
    }

}