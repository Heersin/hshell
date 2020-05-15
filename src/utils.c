/* Utils.c : Command reading and Parsing 
*  some utils functions for develop shell
*
*/

#include "utils.h"

#define	is_delim(x) ((x)==' '||(x)=='\t')

// for some special functions
static char *builtin_argv[MAX_ARGV + 1];
extern char **environ;


char *next_cmd(char *prompt, FILE *fp)
{
    /* Returns buffer of cmd line from fp */
    /* errors : NULL at EOF -- will call fatal error */
    char *buf;
    int bufsize = 0;
    int cur_pos = 0;
    int ch;
    int tmp;
    char *his_string;
    int prompt_len;
    int cur_col = 0;            // current cursor position

    prompt_len = strlen(prompt);

    /* init environ */
    environ = VLtable2environ();
    
    /* Init history cursor */
    init_cursor(0);

    /* Init Buffer */
    buf = emalloc(BUFSIZ);
    bufsize = BUFSIZ;

    printf("%s", prompt);
    while((ch = getc(fp)) != EOF){
        
        /* reach the end of our buffer ? */
        if(cur_pos + 1 >= bufsize){
            buf = erealloc(buf, bufsize + BUFSIZ);
            bufsize += BUFSIZ;
        }

        /* Handle some ctrl char */
        /* This can be function() such as backspace(cur_pos, ch)*/
        switch (ch)
        {
        case CTRL_L:
            builtin_argv[0] = "tput";
            builtin_argv[1] = "clear";
            builtin_argv[2] = NULL;
            execute(builtin_argv);
            printf("%s", prompt);
            for(tmp = 0; tmp < cur_pos; tmp++)
                putc(buf[tmp], stdout);
            break;
        case '\n':
            /* read done */
            /* just break out */
            putc(ch, stdout);   //echo
            cur_col = 0;
            goto cmd_end;
            break;
        case '\b':
            // reach the $>> prompt
            if(cur_pos == 0)
                break;
            putc(ch, stdout);
            putc(' ', stdout);
            putc(ch, stdout);
            cur_pos -= 1;
            cur_col -= 1;

            break;
        case CTRL_D:
            /* Deal with Ctrl-D */
            if(cur_pos == 0)
                printf("Exit Shell\n");
                return NULL;
            break;
        case TAB:
            /* Deal with Tab */
            /* should get buf[0~cur_pos] */
            putc('\n', stdout);
            auto_complete(buf, cur_pos);

            /* reprint cmd */
            printf("%s", prompt);
            for(tmp = 0; tmp < cur_pos; ++tmp)
                putc(buf[tmp], stdout);
            break;
        
        // deal with Arrow key
        case ESC:
            if((ch = getc(fp)) == '[')
            {
                ch = getc(fp);
                switch (ch)
                {
                // up
                case 'A':        
                    his_string = previous_history();

                    for(tmp = 0; tmp < cur_pos; ++tmp){
                        putc('\b', stdout);
                        putc(' ', stdout);
                        putc('\b', stdout);
                    }

                    if(his_string == NULL)
                    {
                        cur_pos = 0;
                        cur_col = 0;
                        break;
                    }

                    cur_pos = strlen(his_string);
                    cur_col = cur_pos;
                    printf("%s", his_string);
                    sprintf(buf, "%s", his_string);

                    break;
                
                case 'B':
                    his_string = next_history();

                    for(tmp = 0; tmp < cur_pos; ++tmp){
                        putc('\b', stdout);
                        putc(' ', stdout);
                        putc('\b', stdout);
                    }

                    // handle the current cmd
                    if(his_string == NULL)
                    {
                        cur_pos = 0;
                        cur_col = 0;
                        break;
                    }

                    cur_pos = strlen(his_string);
                    cur_col = cur_pos;
                    printf("%s", his_string);
                    sprintf(buf, "%s", his_string);
                    break;

                // left
                case 'D':
                    if(cur_col == 0)
                        break;
                    cur_col -= 1;
                    putc(ESC, stdout);
                    putc('[', stdout);
                    putc('D', stdout);
                    break;

                case 'C':
                    if(cur_col == cur_pos)
                        break;
                    cur_col += 1;
                    putc(ESC, stdout);
                    putc('[', stdout);
                    putc('C', stdout);
                    break;

                default:
                    printf("^[");
                    putc(ch, stdout);
                    break;
                }
            }
            break;

        default:
            /* continue to read command char */
            // buf[cur_pos++] = ch;

            putc(ch, stdout);   //echo

            /* insert mode */
            /* the screen part */
            for(tmp = cur_col; tmp < cur_pos; ++tmp){
                putc(buf[tmp], stdout);
            }
            /* command arg part */
            for(tmp = cur_pos; tmp > cur_col; --tmp){
                buf[tmp] = buf[tmp - 1];
                putc('\b', stdout);
            }

            buf[cur_col] = ch;
            cur_col += 1;
            cur_pos += 1;
            break;
        }
    }


cmd_end:
        /* pack cmd, just set a NUL at the end */
        buf[cur_pos] = '\0';
        /* write to history */
        addhistory(buf, cur_pos);
        write2history(NULL);
        free(environ);
        return buf;
}


char **splitline(char *line)
{
    /* Split a line into array of token */
    /* Returns : NULL-terminated array pointer */
    /* Error: NULL if no tokens */

    char **args;
    int argc = 0;
    int cur_max_argc = 0;
    int bufsize = 0;
    int len = 0;

    char *cur_pos;
    char *start;

    // No Commands 
    if(line == NULL)
        return NULL;

    // Init args buffer
    args = emalloc(BUFSIZ);
    bufsize = BUFSIZ;
    cur_max_argc = BUFSIZ/sizeof(char *);
    cur_pos = line;

    while( *cur_pos != '\0')
    {
        // some white space--skip
        while(is_delim(*cur_pos))
            cur_pos++;

        // Means A totally space token
        if(*cur_pos == '\0')
            break;

        // the args buffer has spare room ?
        if(argc + 1 >= cur_max_argc){
            args = erealloc(args, bufsize + BUFSIZ);
            bufsize += BUFSIZ;
            cur_max_argc = bufsize/sizeof(char*);
        }

        // So Here is the start of current argument string
        start = cur_pos;
        len = 1;
        while(*++cur_pos != '\0' && !(is_delim(*cur_pos)))
            len += 1;
        args[argc++] = newstr(start, len);
    }
    args[argc] = NULL;
    return args;


}

char *newstr(char *s, int l)
{
    char *result = emalloc(l + 1);

    strncpy(result, s, l);
    result[l] = '\0';
    return result;
}

void freelist(char **list)
{
    /* Free the spilited line and args in this buf */
    char **cur_pos = list;
    while(*cur_pos)
        free(*cur_pos++);
    free(list);
}

void * emalloc(size_t n)
{
	void *rv ;
	if ( (rv = malloc(n)) == NULL )
		fatalerror("emalloc", "out of memory",-1);
	return rv;
}
void * erealloc(void *p, size_t n)
{
	void *rv;
	if ( (rv = realloc(p,n)) == NULL )
		fatalerror("realloc","realloc() failed",-1);
	return rv;
}


void fatalerror(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}

