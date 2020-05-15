#include "history.h"

/* 很像一个队列，当触及到队列末尾时弹出队首，加入队尾 */
/* queue hdr --> 有东西, queue tail-->空位置 */
int queue_hdr = 0;
int queue_tail = 0;
int queue_len = 0;
int queue_cursor = 0;   // tail -- 0, the latest cmd

char *new_string(char *source, int len)
{
    char *result = malloc(len + 1);
    strncpy(result, source, len);
    result[len] = '\0';
    return result;
}

void init_history(char *filename)
{
    FILE *fp;
    char *buf;
    int bufsize = 0;
    int cur_pos = 0;
    int ch;
    int tmp;

    for(tmp = 0; tmp < MAX_HISTORY; ++tmp)
        history[tmp] = NULL;

    if(filename == NULL){
        if((fp = fopen("./.hsh_history", "r")) == NULL)
        {
            perror("Cannot find your history");
            return -1;
        }
    }

    /* Init Buffer */
    buf = malloc(BUFSIZ);
    if(buf == NULL)
    {
        perror("Cannot Alloc for history buffer");
        return -1;
    }
    bufsize = BUFSIZ;

    
    while( ((ch = getc(fp)) != EOF) && (queue_len < MAX_HISTORY) )
    {
        /* reach the end of our buffer ? */
        if(cur_pos + 1 >= bufsize){
            buf = realloc(buf, bufsize + BUFSIZ);
            bufsize += BUFSIZ;
        }

        if(ch == '\n'){
            // a global history table
            //history[queue_len++] = new_string(buf, cur_pos);
            inqueue(new_string(buf, cur_pos));
            // clear current buf
            cur_pos = 0;
            continue;
        }

        buf[cur_pos++] = ch;
    }
        
    fclose(fp);
    free(buf);
    return history;
}

void free_history(){
    int tmp;
    for(tmp = 0; tmp < MAX_HISTORY; tmp++)
        if(history[tmp] != NULL)
            free(history[tmp]);
}

void inqueue(char *string)
{
    if(queue_len == MAX_HISTORY)
        // queue full
        outqueue();

    history[queue_tail] = string;
    queue_tail = (++queue_tail) % MAX_HISTORY;
    ++queue_len;
    queue_cursor = 0;
}

void outqueue()
{
    queue_hdr = (++queue_hdr) % MAX_HISTORY;
    queue_len -= 1;
}

char *locate_history(int index){
    return history[index];
}

void init_cursor(int cursor){
    queue_cursor = cursor;
}

char *previous_history(){
    queue_cursor += 1;
    if(queue_cursor >= queue_len){
        queue_cursor -= 1;
        return locate_history(queue_hdr);
    }
    // something like magic
    return locate_history((queue_hdr + queue_len - queue_cursor) % MAX_HISTORY);
}

char *next_history(){
    queue_cursor -= 1;
    if(queue_cursor <= 0){
        queue_cursor = 0;
        return locate_history(queue_tail);  //actually return NULL
    }

    return locate_history((queue_hdr + queue_len - queue_cursor) % MAX_HISTORY);
}


void write2history(char *filename)
{
    FILE *fp;
    int tmp;
    char *string;

    if(filename == NULL)
    {
        if((fp = fopen("./.hsh_history", "w+")) == NULL)
        {
            perror("Cannot find your history");
            return -1;
        }
    }
    for(tmp = 0; tmp < queue_len; ++tmp){
        string = locate_history( (queue_hdr + tmp) % MAX_HISTORY);
        fprintf(fp, "%s\n", string);
    }
    
    fclose(fp);
}

void addhistory(char *string, int len){
    char *cmd;
    cmd = new_string(string, len);
    
    inqueue(cmd);
}